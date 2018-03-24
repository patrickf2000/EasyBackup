## EasyBackup

This is a simple backup utility written in C++ using Qt. This allows you to easily backup directories of your choice. EasyBackup mirrors the directories you choose to backup; in other words, the backup copies look exactly like the originals. Below is a list of supported platforms:   

1. Linux
2. Windows (tested on Win7 and Win10)
3. Mac OS
4. BSD (tested on GhostBSD, which is based on FreeBSD)   

On Windows, we use robocopy. On Linux, Mac OS, and BSD, we use rsync. On Linux, Mac OS, and BSD, you can install the application with "make install". On Mac, an app bundle is built and installed to the /Applications directory. Windows has no installation support yet.


