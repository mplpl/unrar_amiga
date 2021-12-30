#include "rar.hpp"

#ifdef _AMIGA
size_t __stack  = 800000;
static const char __attribute((used)) min_stack[] = "$STACK:800000";
#define Q(x) #x
#define QUOTE(x) Q(x)
#if RARVER_MINOR < 10
static const char __attribute((used)) vers[] = "\\0$VER: UnRAR "
		QUOTE(RARVER_MAJOR) ".0" QUOTE(RARVER_MINOR)
        " (16.6.2021)";
#else
static const char __attribute((used)) vers[] = "\\0$VER: UnRAR "
		QUOTE(RARVER_MAJOR) "." QUOTE(RARVER_MINOR)
        " (16.6.2021)";
#endif

#include <proto/exec.h>
int Check_Stack()
{
#ifdef __amigaos4__
	return 0;
#elif defined(__MORPHOS__) && !defined(__warpos__)
	return 0;
#elif defined(__warpos__) 
  printf("\nWARNING: stack must be 800K or more - before running unrar, call: stack 800000\n");
  return 0;
  /*struct TaskPPC *task = FindTaskPPC(NULL);
  printf("Stack check fired %u \n", task->tp_StackSize);
	if (task->tp_StackSize < __stack)
	{
		printf("Stack too small %d - %d bytes is needed\n",
			task->tp_StackSize,
			__stack);
		return 1;
	}
	return 0;*/
#else
  struct Task *task = FindTask(NULL);
  size_t sz = (size_t)task->tc_SPUpper - (size_t)task->tc_SPLower;
  if (sz < __stack)
	{
		printf("Stack too small %d - %d bytes is needed\n",
			sz,
			__stack);
		return 1;
	}
	return 0;
#endif
}

#endif

#if !defined(RARDLL)
int main(int argc, char *argv[])
{
#ifdef _AMIGA
  if (Check_Stack()) return 100;
  Locale_Open("unrar.catalog");
#endif

#ifdef _UNIX
  setlocale(LC_ALL,"");
#endif

  InitConsole();
  ErrHandler.SetSignalHandlers(true);

#ifdef SFX_MODULE
  wchar ModuleName[NM];
#ifdef _WIN_ALL
  GetModuleFileName(NULL,ModuleName,ASIZE(ModuleName));
#else
  CharToWide(argv[0],ModuleName,ASIZE(ModuleName));
#endif
#endif

#ifdef _WIN_ALL
  SetErrorMode(SEM_NOALIGNMENTFAULTEXCEPT|SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);


#endif

#if defined(_WIN_ALL) && !defined(SFX_MODULE)
  // Must be initialized, normal initialization can be skipped in case of
  // exception.
  POWER_MODE ShutdownOnClose=POWERMODE_KEEP;
#endif

  try
  {

    CommandData *Cmd=new CommandData;
#ifdef SFX_MODULE
    wcsncpyz(Cmd->Command,L"X",ASIZE(Cmd->Command));
    char *Switch=argc>1 ? argv[1]:NULL;
    if (Switch!=NULL && Cmd->IsSwitch(Switch[0]))
    {
      int UpperCmd=etoupper(Switch[1]);
      switch(UpperCmd)
      {
        case 'T':
        case 'V':
          Cmd->Command[0]=UpperCmd;
          break;
        case '?':
          Cmd->OutHelp(RARX_SUCCESS);
          break;
      }
    }
    Cmd->AddArcName(ModuleName);
    Cmd->ParseDone();
    Cmd->AbsoluteLinks=true; // If users runs SFX, he trusts an archive source.
#else // !SFX_MODULE
    Cmd->ParseCommandLine(true,argc,argv);
    if (!Cmd->ConfigDisabled)
    {
      Cmd->ReadConfig();
      Cmd->ParseEnvVar();
    }
    Cmd->ParseCommandLine(false,argc,argv);
#endif

#if defined(_WIN_ALL) && !defined(SFX_MODULE)
    ShutdownOnClose=Cmd->Shutdown;
    if (ShutdownOnClose)
      ShutdownCheckAnother(true);
#endif

    uiInit(Cmd->Sound);
    InitLogOptions(Cmd->LogName,Cmd->ErrlogCharset);
    ErrHandler.SetSilent(Cmd->AllYes || Cmd->MsgStream==MSG_NULL);

    Cmd->OutTitle();
    Cmd->ProcessCommand();
    delete Cmd;
  }
  catch (RAR_EXIT ErrCode)
  {
    ErrHandler.SetErrorCode(ErrCode);
  }
  catch (std::bad_alloc&)
  {
    ErrHandler.MemoryErrorMsg();
    ErrHandler.SetErrorCode(RARX_MEMORY);
  }
  catch (...)
  {
    ErrHandler.SetErrorCode(RARX_FATAL);
  }

#if defined(_WIN_ALL) && !defined(SFX_MODULE)
  if (ShutdownOnClose!=POWERMODE_KEEP && ErrHandler.IsShutdownEnabled() &&
      !ShutdownCheckAnother(false))
    Shutdown(ShutdownOnClose);
#endif
#if defined(_AMIGA) && !defined(__mini__)
  if (iconvOpenError)
  {
    mprintf(L"\n");
    mprintf(St(MAmigaConvInitErr), GetCodePageW());
    mprintf(L"\n\n");
  }
  else if (iconvConversionError)
  {
    mprintf(L"\n");
    mprintf(St(MAmigaConvErr), GetCodePageW());
    mprintf(L"\n\n");
  }
#endif
#if defined(_AMIGA)
  ReleaseConvBase();
  Locale_Close();
#endif
  ErrHandler.MainExit=true;
  return ErrHandler.GetErrorCode();
}
#endif
