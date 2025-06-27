# 适用于有道词典笔OS的单词本DeepSeek客户端
本仓库存放适用于有道词典笔OS的单词本DeepSeek客户端的源代码和一键部署脚本（仅适用于S6、X5型号）
仓库包含源代码和适用于S6、X5型号词典笔的一键构建脚本、工具链、链接库。
其他型号的词典笔需要更换词典笔中相应版本的cURL、glibc.

## 使用方法
### 一键部署脚本
在Ubuntu 22.04 LTS上执行`build.sh`
### 客户端的使用
- 将构建完成的`dsProject`二进制文件放入MTP的`/Favorite`文件夹下
- 连接`adb shell`，执行`cd /userdisk/favorite/`
- 执行 `chmod 777 dsProject`
- 执行 `./dsProject`
- 随后，根据提示到目录下的cfg文件中填写提示词、api_key、模型地址和模型名称等关键信息。
- 重新执行 `./dsProject`
- （可选）开机启动：在 `init.d/rcS`中追加以下内容：
  ```
  cd /userdisk/Favorite
./dsProject > /userdisk/Favorite/dsproject.log 2>&1 &
  ```
-连接网络，翻译需要询问的问题，在提问最后追加`@ds`，收藏，等待主程序重启，到单词本中查看回复。
