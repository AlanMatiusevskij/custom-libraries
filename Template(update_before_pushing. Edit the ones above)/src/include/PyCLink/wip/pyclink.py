from ctypes import wintypes
from ctypes import windll
from ctypes import memmove
from ctypes import sizeof
from threading import Thread
import time
import ctypes

#Defining private types for kernel.
class __SECURITY_ATTRIBUTES(ctypes.Structure):
    _fields_ = [('nLength', wintypes.DWORD),
                ('lpSecurityDescriptor', wintypes.LPVOID),
                ('bInheritHandle', wintypes.BOOL)]

__INVALID_HANDLE_VALUE = wintypes.HANDLE(-1).value

__PAGE_READWRITE = 0x04
__FILE_MAP_READ = 0x04
__FILE_MAP_WRITE = 0x02

__LPSECURITY_ATTRIBUTES = ctypes.POINTER(__SECURITY_ATTRIBUTES)
__SIZE_T = ctypes.c_size_t
__kernel32 = ctypes.WinDLL('kernel32', use_last_error=True)

__CreateFileMappingW = __kernel32.CreateFileMappingW
__CreateFileMappingW.argtypes = wintypes.HANDLE, __LPSECURITY_ATTRIBUTES, wintypes.DWORD, wintypes.DWORD, wintypes.DWORD, wintypes.LPCWSTR
__CreateFileMappingW.restype = wintypes.LPVOID
__MapViewOfFile = __kernel32.MapViewOfFile
__MapViewOfFile.restype = wintypes.LPVOID
__MapViewOfFile.argtypes = wintypes.HANDLE, wintypes.DWORD, wintypes.DWORD, wintypes.DWORD, __SIZE_T
__UnmapViewOfFile = __kernel32.UnmapViewOfFile
__UnmapViewOfFile.argtypes = wintypes.LPCVOID,
__UnmapViewOfFile.restype = wintypes.BOOL
__CloseHandle = __kernel32.CloseHandle
__CloseHandle.argtypes = wintypes.HANDLE,
__CloseHandle.restype = wintypes.BOOL

#Private variables and methods
__mainMap = 'PyC-Link-v.01'
__mainHandle = None
__mainData = None

__loop_thread = None

class __COMMANDS:
   LAUNCHED = 0
   PYRUNNING = 1

def __loop(frequency):
   while True:

      time.sleep(1/frequency)

#Public variables and methods

def initialize(loopFrequency):
   """
   Initializes pyclink python side. Creates a thread which waits for instructions.

   Parameters
   -
   `loopFrequency` is how many times per second the main loop will repeat.

   Return
   -
   Returns `False` if succeeded and `True` if failed.
   """

   #Create the main filemap.
   __mainHandle = __CreateFileMappingW(__INVALID_HANDLE_VALUE, None, __PAGE_READWRITE, 0, sizeof(ctypes.c_int), __mainMap)
   #TODO :check if failed
   #TODO : check if it created, it should create one*.

   #Create a pointer to data.
   __mainData = __MapViewOfFile(__mainHandle, __FILE_MAP_WRITE, 0, 0, sizeof(ctypes.c_int))
   #Check if pointer accessed.
   if __mainData == 0:
      return True
   
   #Update c++ of success.
   memmove(__mainData, ctypes.cast((ctypes.c_int8 * 1)(__COMMANDS.PYRUNNING), ctypes.POINTER(ctypes.c_int8)), sizeof(ctypes.c_int))
   
   #Create a thread.
   __loop_thread = Thread(None, __loop, None, [loopFrequency], None)
   Thread.start(__loop_thread)
   #Check if the thread was created.
   if Thread.is_alive(__loop_thread) == False:
      return True

   return False