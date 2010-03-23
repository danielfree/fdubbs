#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/telnet.h>
#include <arpa/inet.h>
#ifdef SYSV
#include <sys/termios.h>
#else
#include <termios.h>
#endif
#ifdef LINUX
#include <sys/ioctl.h>
#endif
#ifdef SSHBBS
#include <libssh/libssh.h>
#include <libssh/server.h>
#endif // SSHBBS
#include "bbs.h"

#ifdef SSHBBS
#define KEYS_FOLDER "/etc/ssh/"
#define PID_FILE	BBSHOME"/reclog/sshbbsd.pid"
#else // SSHBBS
#define PID_FILE	BBSHOME"/reclog/bbsd.pid"
#endif // SSHBBS
#define LOG_FILE	BBSHOME"/reclog/bbsd.log"
#define NOLOGIN		BBSHOME"/NOLOGIN"
#ifdef IP_2_NAME
#define IPNAME_FILE BBSHOME"/etc/hosts"
#endif // IP_2_NAME

enum {
	DEFAULT_PORT = 12345, ///< default listening port number
	SOCKET_QLEN = 4, ///< maximum length of the queue of pending connections
};

extern char fromhost[];

// TODO: deprecate this
char genbuf[1024]; ///< global buffer for strings. 

#ifdef SSHBBS
ssh_channel ssh_chan;
#endif // SSHBBS

// TODO: rewrite this
static void telnet_init() {
	static char svr[] = { IAC, WILL, TELOPT_ECHO, IAC, WILL, TELOPT_SGA };
	struct timeval to;
	int rset = 1;
	char buf[256];

	write_stdout(svr, sizeof(svr));
	to.tv_sec = 6;
	to.tv_usec = 1;
	if (select(1, (fd_set *)(&rset), NULL, NULL, &to)> 0)
		read_stdin(buf, sizeof(buf));
}

/**
 * Get remote ip address.
 * @param from socket struct
 * @note the result is stored in the global variable 'fromhost'.
 */
static void get_ip_addr(struct sockaddr_in *from)
{
	strcpy(fromhost, inet_ntoa(from->sin_addr));
#ifdef IP_2_NAME
	// TODO: rewrite
#endif // IP_2_NAME
}

/**
 * Wait for an child to terminate.
 * @param notused only to match sighandler_t prototype
 */
static void reapchild(int notused)
{
	int state, pid;
	while ((pid = waitpid(-1, &state, WNOHANG | WUNTRACED)) > 0)
		;
}

/**
 * Daemonize the process.
 */
static void start_daemon(void)
{
	int n, pid;

	umask(0); // Clear file creation mask.
	n = getdtablesize(); // Get maximum number of file descriptors.

	// Fork to ensure not being a process group leader.
	if ((pid = fork()) < 0) {
		printf("Cannot fork.\n");
		exit(1);
	} else if (pid != 0) {
		exit(0);
	}
	if (setsid() == -1)
		exit(1);

	// Fork again.
	if ((pid = fork()) < 0) {
		exit(1);
	} else if (pid != 0) {
		exit(0);
	}

	// Close all open file descriptors.
	while (n)
		close(--n);

	// Ignore all signals (except SIGKILL & SIGSTOP of course)
	for (n = 1; n <= NSIG; n++)
		signal(n, SIG_IGN);
}

/**
 * Exit.
 * @param notused only to match sighandler_t prototype
 */
static void close_daemon(int notused)
{
	exit(0);
}

/**
 * Log bbsd events.
 * @param str string to log.
 * @return 0 on success, -1 on error.
 */
static int bbsd_log(const char *str)
{
	char buf[256];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	snprintf(buf, sizeof(buf), "%.2d/%.2d/%.2d %.2d:%.2d:%.2d bbsd[%d]: %s",
			t->tm_year % 100, t->tm_mon + 1, t->tm_mday, t->tm_hour,
			t->tm_min, t->tm_sec, getpid(), str);
	return file_append(LOG_FILE, buf);
}

/**
 * Port Binding.
 * @param xsin the socket structure
 * @param port port to bind
 * @return socket descriptor
 */
static int bind_port(struct sockaddr_in *xsin, int port)
{
	char buf[80];
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int on = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on));
	xsin->sin_family = AF_INET;
	xsin->sin_addr.s_addr = INADDR_ANY;
	xsin->sin_port = htons(port);

	if (bind(sock, (struct sockaddr *)xsin, sizeof(*xsin)) < 0) {
		sprintf(buf, "bbsd %s can't bind to %d: %d\n", __func__, port, errno);
		bbsd_log(buf);
		exit(1);
	}
	if (listen(sock, SOCKET_QLEN) < 0) {
		sprintf(buf, "bbsd %s can't listen to %d\n", __func__, port);
		bbsd_log(buf);
		exit(1);
	}

	sprintf(buf, "started on port %d\n", port);
	bbsd_log(buf);
	return sock;
}

#ifdef NOLOGIN
/**
 * Deny new login if NOLOGIN file exists.
 * @param fd output file descriptor
 * @return 0 if login is permitted, -1 otherwise
 * @note '\\r' is not added, so prompt message is not recommended to exceed
 *       1 line.
 */
static int check_nologin(int fd)
{
	const char banner[] = "本站目前暂停登录，原因如下: \r\n";
	mmap_t m;
	m.oflag = O_RDONLY;
	if (mmap_open(NOLOGIN, &m) == 0) {
		write(fd, banner, sizeof(banner));
		write(fd, m.ptr, m.size);
		mmap_close(&m);
		close(fd);
		return -1;
	}
	return 0;
}
#endif // NOLOGIN

/**
 *
 */
static bool sshbbs_auth(const char *user, const char *password)
{
	if (!strcmp(user, "bbs") && !strcmp(password, "bbs"))
		return true;
	return false;
}

/**
 *
 */
static ssh_channel sshbbs_accept(ssh_bind sshbind, ssh_session session)
{
	ssh_message msg;
	ssh_channel chan = false;
	bool auth = false;
	if (ssh_bind_accept(sshbind, session) == SSH_OK
			&& !ssh_accept(session)) {
		do {
			msg = ssh_message_get(session);
			if (!msg)
				break;
			switch (ssh_message_type(msg)) {
				case SSH_REQUEST_AUTH:
					switch(ssh_message_subtype(msg)) {
						case SSH_AUTH_METHOD_PASSWORD:
							if (sshbbs_auth(ssh_message_auth_user(msg),
									ssh_message_auth_password(msg))) {
								auth = true;
								ssh_message_auth_reply_success(msg, 0);
								break;
							}
						case SSH_AUTH_METHOD_NONE:
						default:
							ssh_message_auth_set_methods(msg, 
									SSH_AUTH_METHOD_PASSWORD);
							ssh_message_reply_default(msg);
							break;
					}
					break;
				default:
					ssh_message_reply_default(msg);
			}
			ssh_message_free(msg);
		} while (!auth);		
	}
	if (!auth) {
		ssh_disconnect(session);
		return NULL;
	}
	
    do {
        msg = ssh_message_get(session);
        if (msg) {
			switch(ssh_message_type(msg)) {
				case SSH_REQUEST_CHANNEL_OPEN:
					if (ssh_message_subtype(msg) == SSH_CHANNEL_SESSION) {
						chan = ssh_message_channel_request_open_reply_accept(msg);
                        break;
                    }
                default:
					ssh_message_reply_default(msg);
            }
            ssh_message_free(msg);
        }
    } while (message && !chan);
	if (!chan) {
        ssh_finalize();
        return NULL;
    }
	return chan;
}

int main(int argc, char *argv[])
{
	int csock, nfds, pid;
	int port = DEFAULT_PORT;
	socklen_t value;
	char buf[STRLEN];

	start_daemon();
	signal(SIGCHLD, reapchild);
	signal(SIGTERM, close_daemon);

#ifdef SSHBBS
	ssh_bind sshbind = ssh_bind_new();
	ssh_session session = ssh_new();
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY,
			KEYS_FOLDER "ssh_host_rsa_key");
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_LOG_VERBOSITY,
			SSH_LOG_NOLOG);
#endif // SSHBBS

	// Port binding
	if (argc > 1)
		port = strtol(argv[1], NULL, 10);
	if (port <= 0)
		port = DEFAULT_PORT;
#ifdef SSHBBS
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, port);
	if (ssh_init() < 0)
		exit(1);
#endif // SSHBBS
	struct sockaddr_in xsin;
	int msock = bind_port(&xsin, port);
	if (msock < 0) {
		exit(1);
	}
#ifdef SSHBBS
	ssh_bind_set_fd(sshbind, msock);
#endif // SSHBBS
	nfds = msock + 1;

	// Give up root privileges.
	setgid((gid_t)BBSGID);
	setuid((uid_t)BBSUID);
	chdir(BBSHOME);
	umask(S_IWGRP | S_IWOTH);

	// Log pid.
	sprintf(buf, "%d\n", getpid());
	unlink(PID_FILE);
	file_append(PID_FILE, buf);

	// Main loop.
	while (1) {
#ifdef SSHBBS
		ssh_chan = sshbbs_accept(sshbind, session);
		if (!ssh_chan)
			continue;
		csock = session->fd;
		getpeername(csock, (struct sockaddr *) &xsin, &value);
#else // SSHBBS
		value = sizeof(xsin);
		csock = accept(msock, (struct sockaddr *) &xsin, &value);
		if (csock < 0)
			continue;
		// TODO: nologin
#ifdef NOLOGIN
		if (check_nologin(csock) < 0) {
			close(csock);
			continue;
		}
#endif // NOLOGIN
#endif // SSHBBS

		pid = fork();
		if (pid == 0) {
			while (--nfds >= 0)
				close(nfds);
			dup2(csock, STDIN_FILENO);
#ifndef SSHBBS
			close(csock);
			dup2(STDIN_FILENO, STDOUT_FILENO);
#endif // SSHBBS
			get_ip_addr(&xsin);
			telnet_init();
			start_client();
		}
		close(csock);
	}
	return 0;
}
