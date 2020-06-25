#include<string.h> /* strchr */
#include<stdio.h> /* printf, fopen, fclose */
#include<stdlib.h> /* getenv */
#include<sys/sysinfo.h> /* uptime,freeram,totalram,procsg */
#include<sys/utsname.h> /* uname, kernel, cpu, hostname (nodename) */
#include<glob.h> /* packages */
#include<unistd.h> /* readlink */
#include<sys/stat.h> /* stat */
int main(void){
static FILE *file;
//static int prof1,prof2;
static char gcc[18],
resolution[10],
clib[33],
clibv[63],
alsa[36],
motherboard[25],
//prof[64],
//release[13],
cpul[60],
ge[12],
root[16],
*cpu;
//do not need sock.
static struct stat sock;
static struct sysinfo sys;
static struct utsname kernel;
static  glob_t packages;
//glob("/var/db/pkg/*/*",GLOB_NOSORT|GLOB_NOESCAPE,NULL,&packages);
//glob("/var/lib/pacman/local/*",GLOB_NOSORT|GLOB_NOESCAPE, NULL, &packages);
sysinfo(&sys);
uname(&kernel);

/* CPU */
 file=fopen("/proc/cpuinfo","r");
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,20,file);
 fgets(cpul,60,file); /* jump to 5th line find a better way */
 fclose(file);
 cpu=&cpul[13];

/* gcc */
 file=popen("gcc --version","r");
 fgets(gcc,18,file);
 pclose(file);

/* root */
//df behaves bad on fedora
 file=popen("df -hTx devtmpfs -x tmpfs|awk 'FNR==2{print$4\"/\"$3\" \"$2}'","r");
 fgets(root,16,file);

/* motherboard */
  file=fopen("/sys/devices/virtual/dmi/id/product_name","r");
  fgets(motherboard,25,file);
  fclose(file);

/* clib */
// file=popen("gcc -dumpmachine","r"); takes too much time, on fedora makes no sense!
// file=popen("/lib/libc.so.6","r"); //4characters needed.
//
/*
 file=popen("gcc -dumpmachine","r");
 fgets(clib,18,file);
 pclose(file);
 switch (clib[strlen(clib)-1]) {
  case 'g':
*/
   strcpy(clib,"glibc ");
//   /lib contains all sorts of shit but not lib.so
// we need last field from it
// just  adjusting it for rawhide for now
   file=popen("/lib64/libc.so.6","r");
   fgets(clibv,63,file);
   pclose(file);
   strcpy(&clib[6],&clibv[53]);
/*
  break;
  case 'u':
   strcpy(clib,"ucblibc");
  break;
  case 'm':
   strcpy(clib,"musl");
  break;
 }
*/

/* Resolution */
 file=fopen("/sys/devices/virtual/graphics/fbcon/subsystem/fb0/device/drm/card0/device/graphics/fb0/virtual_size","r");
 fgets(resolution,10,file);
 fclose(file);
 *strchr(resolution,',')='x';

/* sound card detection */
//not so good way.
 file=popen("aplay -L","r");
 do
  fgets(alsa,18,file);
 while (alsa[0]!='d');
 fgets(alsa,36,file);
 pclose(file);


/* graphic environment */
//Maybe USE XDG_RUNTIME_DIR directly?
// sock does not work in fedora, find better way
  strcpy(ge,"Wayland");

/* For now it does not work
 stat("/run/user/1000/wayland-0",&sock);
 if (sock.st_mode==49645)
 strcpy(ge,"Wayland");
 else {
   stat("/run/user/1000/.X11-unix/X0",&sock);
    if (sock.st_mode==49663)
     strcpy(ge,"X11");
    else
     strcpy(ge,"Framebuffer");
 }
 if(getenv("TERM")[0]=='l')
*/

//Some strings contain \n already
printf("[1;34m           /:-------------:\          %s[1;37m@[1;34m%s\n\
[1;34m        :-------------------::       OS:[m Fedora\n\
[1;34m      :-----------[1;37m/shhOHbmp[1;34m---:\      MotherBoard:[m %s\
[1;34m    /-----------[1;37momMMMNNNMMD  [1;34m---:    Kernel:[m %s %s\n\
[1;34m   :-----------[1;37msMMMMNMNMP[1;34m.    ---:   Clib:[m %s\n\
[1;34m  :-----------[1;37m:MMMdP[1;34m-------    ---\   GCC:[m %s\n\
[1;34m ,------------[1;37m:MMMd[1;34m--------    ---:  Graphic Environment:[m %s\n\
[1;34m :------------[1;37m:MMMd[1;34m-------    .---:  Terminal:[m %s\n\
[1;34m :----    [1;37moNMMMMMMMMMNho[1;34m     .----:  Shell:[m %s\n\
[1;34m :--     .[1;37m+shhhMMMmhhy++[1;34m   .------/  Packages:[m %lu\n\
[1;34m :-    -------[1;37m:MMMd[1;34m--------------:   Root:[m %s\
[1;34m :-   --------[1;37m/MMMd[1;34m-------------;    Uptime:[m %lud,%luh,%lum\n\
[1;34m :-    ------[1;37m/hMMMy[1;34m------------:     Processes:[m %u\n\
[1;34m :--[1;37m :dMNdhhdNMMNo[1;34m------------;      RAM:[m %luMiB/%luMiB\n\
[1;34m :---[1;37m:sdNMMMMNds:[1;34m------------:       CPU:[m %s\
[1;34m :------[1;37m:://:[1;34m-------------::\         GPU:[m Intel HD Graphics 4600\n\
[1;34m :---------------------://           Resolution:[m %s\n\
[1;34m                                     Sound Card:[m %s\n",
getenv("USER"),kernel.nodename,
motherboard,
kernel.sysname,kernel.release,
clib,
&gcc[10],
ge,
getenv("TMUX_PANE")?"tmux":getenv("TERM"),
getenv("SHELL"),
packages.gl_pathc,
root,
sys.uptime/86400,sys.uptime/3600%24,sys.uptime/60%60,
sys.procs,
(sys.totalram-sys.freeram)/1048576,sys.totalram/1048576,
cpu,
resolution,
&alsa[4]);
//remove strings which where "Stable"
return 0;
}
