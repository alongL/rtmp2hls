rtmp2hls
===============================

this program is used to pull rtmp and generate hls (with ffmpeg).
now only run in linux.


# how to use?
##1. get ffmpeg for your platform
you can compile ffmpeg by yourself. refer https://github.com/markus-perl/ffmpeg-build-script  
place ffmpeg in bin dir.

## 2.compile rtmp2hls
>cd src
>make 
rtmp2hls will be generated in project dir.

## 3.run 
./rtmp2hls



# tips
ffmpeg can pull the rtmp source and generate hls by this command.
more info about this command. visit https://ffmpeg.org/ffmpeg-formats.html

>ffmpeg  -f flv -i rtmp://yunyun.com/live/my -vcodec copy -acodec copy -f hls -hls_flags delete_segments -segment_list_size 10 -hls_list_size 5 live/playlist.m3u8






# reference
## 1.httplib
https://github.com/yhirose/cpp-httplib
## 2.srs
https://github.com/ossrs/srs/

