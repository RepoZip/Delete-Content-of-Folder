#include <Windows.h>
#include <tchar.h>
#include <io.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;

BOOL IsDots(const TCHAR* str);
BOOL DeleteDirectory(const TCHAR* sPath);

int main(int argc,char** argv)
{
	DeleteDirectory("C:/Users/Kelum Deshapriya/Documents/Test/"); // give path of file which need to delete content
	cout << "finish deletion of file";

	while(true);
	return 0;
}

BOOL IsDots(const TCHAR* str) {
   if(_tcscmp(str,".") && _tcscmp(str,"..")) return FALSE;
   return TRUE;
}

BOOL DeleteDirectory(const TCHAR* sPath) {
		HANDLE hFind;    // file handle
		WIN32_FIND_DATA FindFileData;
 
		TCHAR DirPath[MAX_PATH];
		TCHAR FileName[MAX_PATH];
 
		_tcscpy(DirPath,sPath);
		_tcscat(DirPath,"\\*");    // searching all files
		_tcscpy(FileName,sPath);
		_tcscat(FileName,"\\");
 
		// find the first file
		hFind = FindFirstFile(DirPath,&FindFileData);
		if(hFind == INVALID_HANDLE_VALUE) return FALSE;
		_tcscpy(DirPath,FileName);
 
		bool bSearch = true;
		while(bSearch) {    // until we find an entry
			if(FindNextFile(hFind,&FindFileData)) {
				if(IsDots(FindFileData.cFileName)) continue;
				_tcscat(FileName,FindFileData.cFileName);
				if((FindFileData.dwFileAttributes &
					FILE_ATTRIBUTE_DIRECTORY)) {
 
					// we have found a directory, recurse
					if(!DeleteDirectory(FileName)) {
						FindClose(hFind);
						return FALSE;    // directory couldn't be deleted
					}
					// remove the empty directory
					RemoveDirectory(FileName);
					_tcscpy(FileName,DirPath);
				}
				else {
					if(FindFileData.dwFileAttributes &
					FILE_ATTRIBUTE_READONLY)
					// change read-only file mode
						_chmod(FileName, _S_IWRITE);
						if(!DeleteFile(FileName)) {    // delete the file
							FindClose(hFind);
							return FALSE;
						}
					_tcscpy(FileName,DirPath);
				}
			}
			else {
				// no more files there
				if(GetLastError() == ERROR_NO_MORE_FILES)
				bSearch = false;
				else {
					// some error occurred; close the handle and return FALSE
					FindClose(hFind);
					return FALSE;
				}
 
			}
 
		}
		FindClose(hFind);                  // close the file handle
 
		return TRUE;
 
}
