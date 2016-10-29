/*
 * FTPD Account Manager
 */

var G_Error      = "<?error found?>";
var G_Error_Msg  = "<?error message?>";

<?setvaronce var:objindex $var:newobjindex?>
var objindex = "<?echo $var:objindex?>";

var G_ftpdCounts = [];
var m = 0;
<?objget :InternetGatewayDevice.X_TWSZ-COM_FTPD.Account. "UserName Password EnableList EnableUpload EnableDownload"
`	G_ftpdCounts[m] = [];
	G_ftpdCounts[m][0] = "$01";//用户名
	G_ftpdCounts[m][1] = "$02";//密码（不是真密码）
	G_ftpdCounts[m][2] = "$03";//浏览权限
	G_ftpdCounts[m][3] = "$04";//上传权限
	G_ftpdCounts[m][4] = "$05";//下载权限
	G_ftpdCounts[m][5] = "$00";//index
	m++;
`?>

var G_Subscript = -1;//用来记住现在编辑的是哪个

//该函数只能在Onload函数中被调用
function uiSubmitOnload(){

	var path = ":InternetGatewayDevice.X_TWSZ-COM_FTPD.Account." + objindex + ".";

	$F(path+"UserName", "<?echo $var:username?>");
	$F(path+"Password", "<?echo $var:password?>");
	$F(path+"EnableList", "<?echo $var:access?>");
	$F(path+"EnableUpload", "<?echo $var:upload?>");
	$F(path+"EnableDownload", "<?echo $var:download?>");
	$F('obj-action','set');
	$F('getpage','html/index.html');
	$F('errorpage','html/index.html');
	$F('var:menu','advanced');
	$F('var:page','ftpd_count_manager');

	$('uiPostForm').submit();

}

function uiSubmit()
{
	if (checkValue() == false)
	{
		return false;
	}

	//添加
	if (G_Subscript == -1)
	{
		uiAddItem();
		return true;
	}

	//编辑
	var path = ":InternetGatewayDevice.X_TWSZ-COM_FTPD.Account." + G_ftpdCounts[G_Subscript][5] + ".";

	$F(path+"UserName", $("INPUT_userName").value);
	$F(path+"Password", $("INPUT_password").value);
	$F(path+"EnableList", Number($("INPUT_access").checked));
	$F(path+"EnableUpload", Number($("INPUT_upload").checked));
	$F(path+"EnableDownload", Number($("INPUT_download").checked));
	$F('obj-action','set');
	$F('getpage','html/index.html');
	$F('errorpage','html/index.html');
	$F('var:menu','advanced');
	$F('var:page','ftpd_count_manager');
	G_Subscript = -1;
	$("add").disabled = true;

	$('uiPostForm').submit();
}

function checkValue(){ // before submit form,check vilidity value

	return true;
}

function uiOnload(){
	//如果是添加之后的动作，则直接提交(当objindex的值为"-"时表示刚添加了一个实例)
	if (objindex != "-")
	{
		uiSubmitOnload();
		return true;
	}

	G_Subscript = -1;

	createTable();
	dealWithError();
}

function uiRefreshPage(){
	document.location.href = "/cgi-bin/webproc?getpage=html/index.html&var:menu=advanced&var:page=ftpd_count_manager";
}

function createTable()
{
	var value_array = [];
	var input_value;
	for(var i = 0; i < G_ftpdCounts.length; i++){
		value_array[i] = [];

		//序号
		value_array[i].push(i+1);

		//用户名
		value_array[i].push(G_ftpdCounts[i][0]);

		//密码
		value_array[i].push(G_ftpdCounts[i][1]);

		//权限之显示
		if (G_ftpdCounts[i][2] == "1")
			value_array[i].push("Y");
		else
			value_array[i].push("N");

		//权限之上传
		if (G_ftpdCounts[i][3] == "1")
			value_array[i].push("Y");
		else
			value_array[i].push("N");

		//权限之下载
		if (G_ftpdCounts[i][4] == "1")
			value_array[i].push("Y");
		else
			value_array[i].push("N");

		//操作。注意，i同时也是G_ftpdCounts的下标，很有用
		//编辑
		input_value = '<input type="button" name="edit" id="edit" value="Edit" onclick="uiEdit(' + i + ')"/ class="my_btn">';
		//删除
		input_value += '<input type="button" name="del" id="del" value="Delete" onclick="uiDelete(' + i + ')"/ class="my_btn">';

		value_array[i].push(input_value);
	}

	$T('tb_multiIPList',value_array);
}

function uiEdit(varSubscript)
{
	$("add").value = "Modify";

	G_Subscript = varSubscript;

	$("INPUT_userName").disabled = true;

	$("INPUT_userName").value = G_ftpdCounts[varSubscript][0];
	$("INPUT_password").value = G_ftpdCounts[varSubscript][1];
	$("INPUT_access").checked = Number(G_ftpdCounts[varSubscript][2]);
	$("INPUT_upload").checked = Number(G_ftpdCounts[varSubscript][3]);
	$("INPUT_download").checked = Number(G_ftpdCounts[varSubscript][4]);
}

function uiAddItem()
{
	$F('obj-action','add');
	$F('add-obj',"InternetGatewayDevice.X_TWSZ-COM_FTPD.Account.");
	$F('getpage','html/index.html');
	$F('errorpage','html/index.html');
	$F('var:menu','advanced');
	$F('var:page','ftpd_count_manager');
	$F('var:username',$("INPUT_userName").value);
	$F('var:password',$("INPUT_password").value);
	$F('var:access',Number($("INPUT_access").checked));
	$F('var:upload',Number($("INPUT_upload").checked));
	$F('var:download',Number($("INPUT_download").checked));
	$('uiPostForm').submit();
}

function uiDelete(varSubscript)
{
	$F('del-obj',"InternetGatewayDevice.X_TWSZ-COM_FTPD.Account."+ G_ftpdCounts[varSubscript][5] + ".");
	$F('obj-action','del');
	$F('getpage','html/index.html');
	$F('errorpage','html/index.html');
	$F('var:menu','advanced');
	$F('var:page','ftpd_count_manager');
	$('uiPostForm').submit();
}

function dealWithError()
{
	if (G_Error != 1)
	{
		return false;
	}

	var arrayHint = [];

	getHintString(arrayHint, G_Error_Msg);
}

addListeners(uiOnload, dealWithError);
