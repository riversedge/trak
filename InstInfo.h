#ifndef INSTINFO_H
#define INSTINFO_H

#include "stdafx.h"

class CInstanceInfo {
public:
	CString Filename;		// Filename
	CString Filepath;		// Path
	CString BaseName;		// Base filename
	DWORD   RecDev;			// Record device number
	DWORD   PlayDev;		// Play device number
	DWORD   PlayBuff;		// Play buffer size
	DWORD   NumPlayBuff;	// Number play buffers
	DWORD   RecBuff;		// Record buffer size
	DWORD   SyncPlay;		// Is syncronous playback enabled
	DWORD   SampRate;		// Sample rate
	DWORD   SampBits;		// Sample bits
	BOOL	FilledPrefs;	// Filled preferences
	BOOL    FilledStart;	// Filled startup info

public:
	CInstanceInfo();
	~CInstanceInfo();
};

#endif