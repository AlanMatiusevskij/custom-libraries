#this is an example of how you could do this by yourself
#Essential
from ctypes import wintypes
from ctypes import windll
import ctypes

#this value should be the same for c++
fileMapName = 'fileMapTest'
MAXcharNUMB = 1000

#Credit goes to Mark Tolonen: https://stackoverflow.com/a/78476033/23690064

class SECURITY_ATTRIBUTES(ctypes.Structure):
    _fields_ = [('nLength', wintypes.DWORD),
                ('lpSecurityDescriptor', wintypes.LPVOID),
                ('bInheritHandle', wintypes.BOOL)]

INVALID_HANDLE_VALUE = wintypes.HANDLE(-1).value

PAGE_READWRITE = 0x04

FILE_MAP_READ = 0x04
FILE_MAP_WRITE = 0x02

LPSECURITY_ATTRIBUTES = ctypes.POINTER(SECURITY_ATTRIBUTES)
SIZE_T = ctypes.c_size_t
kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)

CreateFileMappingW = kernel32.CreateFileMappingW
CreateFileMappingW.argtypes = wintypes.HANDLE, LPSECURITY_ATTRIBUTES, wintypes.DWORD, wintypes.DWORD, wintypes.DWORD, wintypes.LPCWSTR
CreateFileMappingW.restype = wintypes.LPVOID
MapViewOfFile = kernel32.MapViewOfFile
MapViewOfFile.argtypes = wintypes.HANDLE, wintypes.DWORD, wintypes.DWORD, wintypes.DWORD, SIZE_T
MapViewOfFile.restype = wintypes.LPVOID
UnmapViewOfFile = kernel32.UnmapViewOfFile
UnmapViewOfFile.argtypes = wintypes.LPCVOID,
UnmapViewOfFile.restype = wintypes.BOOL
CloseHandle = kernel32.CloseHandle
CloseHandle.argtypes = wintypes.HANDLE,
CloseHandle.restype = wintypes.BOOL

#MAXcharNUMB not multiplied by 8 (char size?)
fileMappingObject = CreateFileMappingW(INVALID_HANDLE_VALUE, None, PAGE_READWRITE, 0, MAXcharNUMB, fileMapName)
mapData = MapViewOfFile(fileMappingObject, FILE_MAP_READ, 0, 0, MAXcharNUMB)

if(mapData == 0):
    print("Couldn't access or open data")
else:
    data = ctypes.string_at(mapData).decode()
    print(data)

UnmapViewOfFile(mapData)
CloseHandle(fileMappingObject)