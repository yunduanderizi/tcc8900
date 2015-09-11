1. 将qmake路径加入到PATH中，使用tcc890x_common库中trunk/packages/target/qt4.6.2/bin下的qmake。
为了使用这个qmake，需要在qmake所在目录下生成一个文件qt.conf，文件内容为：

[Paths]

Documentation = doc

Headers = include

Libraries = lib

Binaries = bin

Plugins = plugins

Translations = translations

Prefix = /home/xingqq/svndir/tcc8900/trunk/3rdpartyPackage/qt4.6.2

其中Prefix修改为正确的路径

2. 打开product.conf文件：
调整如下参数
    PACKAGE_DIR 第3方库的目录，就是tcc890x_common库中的trunk/packages/target所在的目录
    INNOVBOX_INSTALL_DIR 编译后的安装目录
    
3. 在本目录下，qmake, make即可。