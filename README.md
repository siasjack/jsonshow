# jsonshow
parase json data for shell script
if u have any question,come to here ask
https://www.openwrtdl.com/wordpress/jsonshowshell%e5%a4%84%e7%90%86json%e6%95%b0%e6%8d%ae%e5%88%a9%e5%99%a8

useage:  
$ ./jsonshow -h  
jsonshow(V0.1.0),build time:20180813  
	-s 'json string',cant use -f option    
	-f 'json file path',cant use -s option  
	-c 'parase cmd',ex:root.name root.info.user(must be start with root)  
	-b '0/1',print beauty,this is useful for shell script(default 1)  
	ex:jsonshow -s '{"name":"jack"}' -c root.name  
	   jsonshow -s '{"name":"jack","arr":[1,2,3,4]}' -c root.arr.#3  
	   jsonshow -f test.json -c root.links.#1.name (#num for array type,first is #0)  
	   jsonshow -f test.json -c root.stu.jack.age  
copywrite (c) jk110333@126.com  

