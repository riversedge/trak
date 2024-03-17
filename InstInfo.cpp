#include "InstInfo.h"

// Constructor
CInstanceInfo::CInstanceInfo() {
	Filename = "";
	Filepath = "";
	BaseName = "";
	RecDev = 0;
	PlayDev = 0;
	PlayBuff = 0;
	NumPlayBuff = 0;
	RecBuff = 0;
	SyncPlay = 0;
	SampRate = 0;
	SampBits = 0;
	FilledPrefs = FALSE;
	FilledStart = FALSE;
};

// Destructor
CInstanceInfo::~CInstanceInfo() {};
