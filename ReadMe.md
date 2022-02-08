rtmp2hls
===============================

this program is used to pull rtmp and generate hls (with ffmpeg).
now only run in linux.


# How to use?
## 1. get ffmpeg for your platform  
you can compile ffmpeg by yourself. refer https://github.com/markus-perl/ffmpeg-build-script  
place ffmpeg in bin dir.

## 2.compile rtmp2hls 
```
cd src
make 
```
rtmp2hls will be generated in project dir.  
It's the same for centos and ubuntu linux.  
you can develop with vs2015 use vs/rtmp2hls.sln, but this program can not run sucess in windows.   

## 3.run 
./rtmp2hls
If ffmpeg process is killed by other process, it will be restarted in 3 seconds.  
If rtmp2hls is stoped by pkill, it will also stop ffmpeg process.  

## 4.config
you can modify tasks.csv to add rtmp to hls tasks.  
for example: pull rtmp://abc.com/live/my to generate /live/my/hls.m3u8 .the csv is like this:  
src is : rtmp://abc.com/live/my  
dest is : /live/my  
hls adress is: http://localhost:8086/live/my/hls.m3u8


# Tips
ffmpeg can pull the rtmp source and generate hls by this command.  
more info about this command. visit https://ffmpeg.org/ffmpeg-formats.html

I have compiled  ffmpeg and placed it in bin dir.  
It can be used in centos 7.6 and ubuntu 18.04  
The cmd used is like this:   
```
ffmpeg  -f flv -i rtmp://yunyun.com/live/my -vcodec copy -acodec copy \
-f hls -hls_flags delete_segments -segment_list_size 5 -hls_list_size 10 live/playlist.m3u8
```

-segment_list_size should not too small.  when client play the stream, client is easy to read *.ts is not in .m3u8




# Reference
This project used some code from these repositories  
 1.httplib  
https://github.com/yhirose/cpp-httplib  
 2.srs  
https://github.com/ossrs/srs/  
 3.csv  
 https://github.com/vincentlaucsb/csv-parser  
