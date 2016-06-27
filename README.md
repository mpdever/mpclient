# MpClient使用指引

---

[toc]

---

## Project结构
> .  
> |-- 3rd  
> |-- include  
> |-- mybuild  
> |-- proto  
> |-- protobuf  
> |-- sample  
> |-- src  
> |-- unittest  
> `-- win32  

其中，  
3rd目录下为引用的第三方开源代码，包括gtest，protobuf  
include目录为引用的头文件  
mybuild目录用于存放构建结果  
sample目录用于存放样例  
src目录用于存放主要的源代码  
unittest目录用于存放单元测试  
proto目录用于存放项目的protobuf结构体定义
win32目录用于存放windows下的一些编译依赖及头文件  

---

##使用及依赖  
项目用了cmake来描述依赖项。  

### Linux  
Linux下，可以在根目录通过以下操作来完成构建：  
cmake .  
make  
如果没有libcurl，则需要自己安装libcurl和openssl(或者其他ssl)  

### Windows  
Windows下，也可以使用cmake来生成nmake，也可以用cmake来生成vs project，对于windows下，我们提供了32位的release版的libcurl, openssl的动态库，放在win32目录下，如果需要选用不同的版本，或者不同的编译选项，可以自己编译库，并将相应头文件放到include文件夹下

---

## TestCase  
使用了第三方的gtest作为主要的测试框架，其中，由于一些业务相关的testcase涉及到账号，权限，业务等信息，所以并不通用，可以将unittest_comm.cpp中的一些信息如appid，appsecret改为测试账号的appid、appsecret，并进行测试，可以直接测试的testcase:  
unittest_basic  
建议根据实际情况，在不影响正常业务的情况下编写自己的testcase


---

##Log
设置了log选项后，MpClient会在执行过程中打印日志。  
log选项包括log级别、log大小限制、log目录。
log级别包括ERROR、WARN、INFO、DEBUG；
设置了log目录后，会在目录下打印日志，每小时一个日志文件，日志文件为{时间.log}，如2016050100.log；
每个日志不超过大小限制，这个大小限制为0-2GB。
日志的格式如下

>[级别] [时间] [接口] [loggerid] {file} {func} {line} {log内容}

其中loggerid为一个日志类初始化时生成的随机数，不同的client有一个不同的日志类，进而拥有不同的loggerid。

---

##多线程
如果你要在多线程环境下使用接口，需要注意以下几点：
- 每个线程使用一个独有的MpClient对象
- 如果使用了openssl，请确保openssl编译时加了threads选项，同时，需要注册一个回调锁对象，见sample
- 初始化时调用MpClient::GlobalInit() ,请确保在主线程完成此操作并只调用一次

---

##依赖的库及其版本
Protobuf 2.6.1
Gtest
Rapidjson
Libcurl
Openssl

---

##数据结构
所有接口的req和resp都为protobuf生成的结构体，具体结构体字段定义可以看proto/mpmsg.proto. 生成的类使用方法可以见sample。

---

## 业务接口一览

接口 | 目录 |  说明
---|---|---
MpClient.Token | /cgi-bin/token |  获取access_token
MpClient.GetCallbackIP | /cgi-bin/getcallbackip |  获取服务器ip
MpClient.GetCurrentAutoReplyInfo | /cgi-bin/get_current_autoreply_info |  获取当前自动回复
MpClient.GetCurrentSelfMenuInfo | /cgi-bin/get_current_selfmenu_info |  获取当前当前自定义菜单
MpClient.CreateMenu | /cgi-bin/menu/create |  创建自定义菜单
MpClient.GetMenu | /cgi-bin/menu/get |  获取自定义菜单
MpClient.DeleteMenu | /cgi-bin/menu/delete |  删除自定义菜单
MpClient.AddConditionalMenu | /cgi-bin/menu/addconditional |  新增个性化菜单
MpClient.DelConditionalMenu | /cgi-bin/menu/delconditional |  删除个性化菜单
MpClient.TryMatchMenu | /cgi-bin/menu/trymatch |  测试个性化菜单
MpClient.UploadImg | /cgi-bin/media/uploadimg |  上传图片
MpClient.UploadNews | /cgi-bin/media/uploadnews |  上传图文
MpClient.UploadMedia | /cgi-bin/media/upload |  上传临时素材
MpClient.GetMedia | /cgi-bin/media/get |  下载临时素材
MpClient.AddNewsMaterial | /cgi-bin/material/add_news |  新增永久图文素材
MpClient.AddMaterial | /cgi-bin/material/add_material |  新增永久素材
MpClient.GetMaterial | /cgi-bin/material/get_material |  获取永久素材
MpClient.DelMaterial | /cgi-bin/material/del_material |  删除永久素材
MpClient.UpdateNews | /cgi-bin/material/update_news |    更新图文
MpClient.GetMaterialCount | /cgi-bin/material/get_materialcount |  获取素材总数
MpClient.BatchGetMaterial | /cgi-bin/material/batchget_material |  获取素材列表
MpClient.SendCustomMessage | /cgi-bin/message/custom/send |  发送客服消息
MpClient.MassSendAll | /cgi-bin/message/mass/sendall |  根据标签群发
MpClient.MassSend | /cgi-bin/message/mass/send |   根据openid列表群发
MpClient.MassDelete | /cgi-bin/message/mass/delete |  删除群发
MpClient.MassPreview | /cgi-bin/message/mass/preview |  群发预览
MpClient.MassGet | /cgi-bin/message/mass/get |  查询群发状态
MpClient.SetIndustry | /cgi-bin/template/api_set_industry |  设置行业信息
MpClient.GetIndustry | /cgi-bin/template/get_industry |  获取所在行业信息
MpClient.AddTemplate | /cgi-bin/template/api_add_template |  新增模板
MpClient.GetAllPrivateTemplate | /cgi-bin/template/get_all_private_template |  获取模板列表
MpClient.DelPrivateTemplate | /cgi-bin/template/del_private_template |  删除模板
MpClient.SendTemplateMessage | /cgi-bin/message/template/send |  发送模板消息
MpClient.CreateTags | /cgi-bin/tags/create |  创建标签
MpClient.GetTags | /cgi-bin/tags/get |   获取公众号所有标签
MpClient.UpdateTags | /cgi-bin/tags/update |  编辑标签
MpClient.DeleteTags | /cgi-bin/tags/delete |  删除标签
MpClient.GetTagUser | /cgi-bin/user/tag/get |  获取标签下的用户列表
MpClient.BatchTaggingMembers | /cgi-bin/tags/members/batchtagging |  批量打标签
MpClient.BatchUnTaggingMembers | /cgi-bin/tags/members/batchuntagging |  批量取消标签
MpClient.GetTagsIdList | /cgi-bin/tags/getidlist |  获取用户标签列表
MpClient.UpdateUserInfoRemark | /cgi-bin/user/info/updateremark |  设置用户备注
MpClient.UserInfo | /cgi-bin/user/info | 获取用户信息
MpClient.BatchGetUserInfo | /cgi-bin/user/info/batchget | 批量获取用户信息
MpClient.GetUser | /cgi-bin/user/get |  获取用户列表


---
