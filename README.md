## EasyBackup

This is a simple backup utility written in C++ using Qt. This allows you to easily backup directories of your choice. EasyBackup mirrors the directories you choose to backup; in other words, the backup copies look exactly like the originals. Below is a list of supported platforms:   

1. Linux
2. Windows (tested on Win7 and Win10)
3. Mac OS
4. BSD (tested on GhostBSD, which is based on FreeBSD)   

On Windows, we use robocopy. On Linux, Mac OS, and BSD, we use rsync. On Linux, Mac OS, and BSD, you can install the application with "make install". On Mac, an app bundle is built and installed to the /Applications directory. Windows has no installation support yet.

#### Translations
This progam has internationalization support through my trans library. By default, it uses the user's locale. The translation is file is located in /usr/share/EasyBackup. You can specify the path to another translation file through the --trans command line option. You can specify the language through the --lang command line option. Or, you can set the $LANG variable. If you wish to see a list of supported languages, simply view the translations file.

Internationalization support is still in its early stages. It will be improved over time.


