rtmp2hls
===============================

this program is used to pull rtmp and generate hls (with ffmpeg).
now only run in linux.


# how to use?
## 1. get ffmpeg for your platform  
you can compile ffmpeg by yourself. refer https://github.com/markus-perl/ffmpeg-build-script  
place ffmpeg in bin dir.

## 2.compile rtmp2hls 
```
cd src
make 
```
rtmp2hls will be generated in project dir.

## 3.run 
./rtmp2hls

## 4.config
you can modify tasks.csv to add rtmp to hls tasks .
src is : rtmp://abc.com/live/my
dest is : live/my   
hls adress is: http://localhost:8086/live/my/hls.m3u8


# tips
ffmpeg can pull the rtmp source and generate hls by this command.
more info about this command. visit https://ffmpeg.org/ffmpeg-formats.html

```
ffmpeg  -f flv -i rtmp://yunyun.com/live/my -vcodec copy -acodec copy \
-f hls -hls_flags delete_segments -segment_list_size 5 -hls_list_size 10 live/playlist.m3u8
```





# reference
this project used some code from these repositories
 1.httplib
https://github.com/yhirose/cpp-httplib
 2.srs
https://github.com/ossrs/srs/
 3.csv
 https://github.com/vincentlaucsb/csv-parser
