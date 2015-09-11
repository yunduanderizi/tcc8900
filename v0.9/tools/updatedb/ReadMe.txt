说明：
编译：	qmake 
		make

功能：把8900旧行标格式的数据库升级为新行标格式

操作的数据库：drivers.db locstatus1.dbxxx locstatus2.dbxxx vdrdeploy.db vdrsetting.db acceleration.db

过程：从旧行标数据库中读取数据，以新行标格式写进【临时目录】中

程序用法： 直接执行程序不跟参数 或者 ./updatedb -help都可调出程序用法
--------------------------------------------------------------------------------------------------------
Usage: ./updatedb [-help][-h]
            [-quiet][-q][-mode 0|1|2|3]
            [-env1 environment_variable_1_path][env2 environment_variable_2_path]
            [-dir1 tmp_nand2_tables_dir] [-dir2 tmp_nand3_tables_dir]

            [-loc1 number] [-loc2 number]

            -h, -help: print usage
            -q, -quiet: don't print log
            -mode, -m: 0 ---> change all types tables
                       1 ---> only change single  tables
                       2 ---> only change multi tables
                       3 ---> only change large tables
            -env1: 
                   environment_variable_1_path: the value of INNOV_IMPORTANT_TABLE_DIR
            -env2: 
                   environment_variable_2_path: the value of INNOV_TABLE_DIR
            -dir1: 
                   tmp_nand2_tables_dir: a path which temporary store the converted database of nand2_tables
            -dir2: 
                   tmp_nand3_tables_dir: a path which temporary store the converted database of nand3_tables
            -loc1: 
                   number: The number needed to convert the locstatus1.db table
            -loc2: 
                   number: The number needed to convert the locstatus2.db table
Example: 
        ./updatedb -mode 1 -env1 /nand2/tables/ -env2 /nand4/tables/ -dir1 /tmp1/ -dir2 /tmp2/ -loc1 50 -loc2 5
--------------------------------------------------------------------------------------------------------

注意事项： 如果选择mode=3(也就是操作locstatus1.dbxxx locstatus2.dbxxx), 建议把操作多少locstatus1.db和locstatus2.db通过参数传递给程序，
			-loc1 50 -loc2 5 表示读取车机locstatus1的最近的50*100条记录 和 locstatus2的最近的5*100条记录。
		
			程序运行时，会先清空 临时存放单表的目录 和 临时存放多表和大表的目录。

【一定要操作完所有类型的数据库后 再覆盖掉原有数据库】

默认值：
		mode:
				默认为0，处理所有类型的tables。
				为1		 处理/nand2/tables/下的单表，也就是vdrdeploy.db vdrsetting.db acceleration.db
				为2		 处理/nand3/tables/下的多表，也就是drivers.db
				为3		 处理/nand3/tables/下的大表，也就是locstatus1.dbxxx locstatus2.dbxxx
				
		环境变量：
		INNOV_IMPORTANT_TABLE_DIR = "/nand2/tables/"		可通过 -env1 指定
		INNOV_TABLE_DIR			  = "/nand4/tables/"		可通过 -env2 指定
		
		临时存放单表的目录：		"/nand2/tmp/tmp_nand2_tables/"		可通过 -dir1 指定
		临时存放多表和大表的目录：	"/nand2/tmp/tmp_nand3_tables/"		可通过 -dir2 指定


