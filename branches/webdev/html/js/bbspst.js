var objPost = document.getElementById('btnPost');
document.onkeydown = function(evt) {
	document.onkeypress = function() {return true;};
	evt = evt ? evt : event;
	if ((evt.keyCode == 119 || evt.keyCode == 87) && evt.ctrlKey) {
		document.onkeypress = function() {return false;};
		document.postform.submit();
		return false;
	}
};
function wait_post()
{
	objPost.disabled=true;
	objPost.value = '���ڷ���,�����ĵȴ�...';
	setTimeout('enable_post()', 20000);
}
function enable_post()
{
	objPost.value = '���ͳ�ʱ,������';
	objPost.disabled = false;
}
function opnewwin()
{
	var mywin = window.open('bbspreupload?board=' + document.getElementById('board').innerHTML, '_blank', 'width=600,height=300,scrollbars=yes');
	if ((document.window != null) && (!mywin.opener))
	mywin.opener = document.window;
	mywin.focus();
	return false;
}