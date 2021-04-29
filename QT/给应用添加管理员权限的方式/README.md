# 给应用添加管理员权限的方式
给MinGW环境下QT编写的程序添加启动的管理员权限方法

1.采用 .rc文件 加上 .manifest配置文件给可执行文件添加管理员权限

2.在pro工程文件中加入

	RC_FILE=main.rc

rc文件中书写如下内容：

	1 24 uac.manifest

.manifest文件中书写如下内容：

	<?xml version='1.0' encoding='UTF-8' standalone='yes'?> 
	<assembly xmlns='urn:schemas-microsoft-com:asm.v1' manifestVersion='1.0'> 
	<trustInfo xmlns="urn:schemas-microsoft-com:asm.v3"> 
	<security> 
	<requestedPrivileges> 
	<requestedExecutionLevel level='requireAdministrator' 	uiAccess='false' /> 
	</requestedPrivileges> 
	</security> 
	</trustInfo> 
	</assembly> 

完成后重新编译，即可在可执行文件图标中看到盾牌标志。


参考链接：https://blog.csdn.net/liukang325/article/details/53261346

题外：inno setup 给安装包添加管理员权限

拥有管理员权限的应用在打包后进行安装，如果使用最后一步安装包的直接启动

windows有时会提示CreateProcess failed；code 740 请求的操作需要提升

这是由于安装包的权限不足，此时需要提升安装包的权限

添加方法链接：https://blog.csdn.net/chenqk_123/article/details/109486575