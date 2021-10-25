#include "stdafx.h"


#include "SyncFileWriter.h"


SyncFileWriter::SyncFileWriter() :
	m_pFile(NULL)
{
#ifdef USE_MULTICORE	
	InitializeCriticalSection(& m_csFile);
#endif
}


SyncFileWriter::~SyncFileWriter()
{
	 closeFile();

#ifdef USE_MULTICORE
	DeleteCriticalSection(& m_csFile);
#endif
}


SyncFileWriter::SyncFileWriter(const SyncFileWriter & rhs)
{
	if( this == &rhs ) 
		return;

	m_pFile = NULL;

	copyFromOther(rhs);

#ifdef USE_MULTICORE	
	InitializeCriticalSection(& m_csFile);
#endif
}

void SyncFileWriter::closeFile()
{
	if (m_pFile == NULL)
		return;

	fflush(m_pFile);
	fclose(m_pFile);
	m_pFile = NULL;
}

void SyncFileWriter::copyFromOther(const SyncFileWriter & rhs)
{
	m_sFullFileName = rhs.m_sFullFileName;
}


bool SyncFileWriter::IsActive() const
{
	return (m_pFile != NULL);
}

const char * SyncFileWriter::getFullFileName() const
{
	return m_sFullFileName.c_str();
}

bool SyncFileWriter::createFile(
	__in const char * szOutputDirectory, 
	__in const char * szOutputFileName, // with or without .csv or other extention
	__in const char * szExtention,      // ".csv", ".ok" ...
    __out std::string & sErrorMsg)
{
	bool bCreateNewFile = true;
	return appendOrCreateFile(__in szOutputDirectory, 
							  __in szOutputFileName, // with or without .csv or other extention
							  __in szExtention,      // ".csv", ".ok" ...
							  __in bCreateNewFile,
							  __out sErrorMsg);
}

bool SyncFileWriter::appendToFile(
	__in const char * szOutputDirectory, 
	__in const char * szOutputFileName, // with or without .csv or other extention
	__in const char * szExtention,      // ".csv", ".ok" ...
	__out std::string & sErrorMsg)
{
	bool bCreateNewFile = false;
	return appendOrCreateFile(__in szOutputDirectory, 
							  __in szOutputFileName, // with or without .csv or other extention
							  __in szExtention,      // ".csv", ".ok" ...
							  __in bCreateNewFile,
							  __out sErrorMsg);
}

bool SyncFileWriter::appendOrCreateFile(
	__in const char * szOutputDirectory, 
	__in const char * szOutputFileName,     // with or without .csv or other extention
	__in const char * szRequiredExtention,  // ".csv", ".ok" ...
	__in bool bCreateNewFile,
	__out std::string & sErrorMsg)
{

	if (_mkdir( szOutputDirectory ) == ENOENT)
	{
		_snprintf_s(m_szErrorMsg, _TRUNCATE, "%s:%d Failed to create DirectoryName =%s<< \n", 
				 __FILE__, __LINE__, szOutputDirectory); 

		sErrorMsg = m_szErrorMsg;
		return false;
	}

	char szFileName[MAX_PATH];
	strcpy_s(szFileName, szOutputFileName);
	{
		const size_t nPrefixLen = strlen(szRequiredExtention);
		char szCurrentExtention[MAX_PATH];
		size_t nFileNameLen = strlen(szFileName) ;
		if (nFileNameLen >= nPrefixLen)
		{
			strcpy_s(szCurrentExtention, szFileName + nFileNameLen - nPrefixLen);
			szCurrentExtention[nPrefixLen] = '\0';

			if (_stricmp(szCurrentExtention, szRequiredExtention) != 0)
			{
				strcat_s(szFileName, szRequiredExtention);
			}
		}
	}

	char szFullFileName[MAX_PATH];
	if (sprintf_s(szFullFileName, "%s\\%s", szOutputDirectory, szFileName) < 0)
	{
		_snprintf_s(m_szErrorMsg, _TRUNCATE, "%s:%d szFullFileName =%s is too long \n", 
					__FILE__, __LINE__, szFullFileName); 
		sErrorMsg = m_szErrorMsg;
		return false;
	}

	m_sFullFileName = szFullFileName;

	closeFile();

	const char * szMode = NULL;
	if (bCreateNewFile)
		szMode = "wtc";
	else
		szMode = "atc";

	if ( fopen_s(& m_pFile, szFullFileName, szMode) != 0 )
	{
		if ( fopen_s(& m_pFile, szFullFileName, szMode) != 0 )
		{
			_snprintf_s(m_szErrorMsg, _TRUNCATE, "%s:%d Could not create file =%s \n", 
						__FILE__, __LINE__, szFullFileName); 
			sErrorMsg = m_szErrorMsg;
			return false;
		}
	}

	return true;
}

bool SyncFileWriter::write(const char * szFormat,  // (I) printf style format string.
                          ...)                  // (I) Variable arguments. 
{
	if (m_pFile == NULL)
		return false;

#ifdef USE_MULTICORE
	EnterCriticalSection(& m_csFile);
#endif

	va_list parmInfo;

	// Initialize variable arguments. 
    va_start(parmInfo, szFormat);

    bool bResult = writeWithFormat (szFormat, parmInfo);

    va_end(parmInfo);

#ifdef USE_MULTICORE
	LeaveCriticalSection(& m_csFile);
#endif
	return bResult;
}

bool SyncFileWriter::writeWithFormat(const char * szFormat,  va_list parminfo)
{
	if (m_pFile == NULL)
		return false;

	vsprintf_s(m_szInputBuffer, szFormat, parminfo);

	if (fputs(m_szInputBuffer, m_pFile) == EOF)
		return false;

	fflush(m_pFile);
	return true;
}

bool SyncFileWriter::printStrings(
	__in const char * szFirstColumn, 
	__in const std::vector<std::string> & values,
	__out std::string & sErrorMsg)
{
	if (m_pFile == NULL)
		return true;

#ifdef USE_MULTICORE
	EnterCriticalSection(& m_csFile);
#endif

	if (fputs(szFirstColumn, m_pFile) == EOF)
	{
		_snprintf_s(m_szErrorMsg, _TRUNCATE, "%s:%d Could not write to %s \n", 
					__FILE__, __LINE__, m_sFullFileName.c_str()); 
		sErrorMsg = m_szErrorMsg;

#ifdef USE_MULTICORE
		LeaveCriticalSection(& m_csFile);
#endif
		return false;
	}

	fputs(",", m_pFile);

	std::string sDate;
	for (std::vector<std::string>::const_iterator iter = values.begin(); iter != values.end(); iter++ )
	{
		const std::string & sValue = *iter;

		if (fputs(sValue.c_str(), m_pFile) == EOF)
		{
			_snprintf_s(m_szErrorMsg, _TRUNCATE, "%s:%d Could not write to %s \n", 
						__FILE__, __LINE__, m_sFullFileName.c_str()); 
			sErrorMsg = m_szErrorMsg;

#ifdef USE_MULTICORE
			LeaveCriticalSection(& m_csFile);
#endif
			return false;
		}
	
		fputs(",", m_pFile);
	}

	fputs("\n", m_pFile);

#ifdef USE_MULTICORE
	LeaveCriticalSection(& m_csFile);
#endif
	return true;
}
 
bool SyncFileWriter::printDoubles(__in const char * szFirstColumn, const std::vector<double> & values,
	__out std::string & sErrorMsg)
{
	if (m_pFile == NULL)
		return true;

#ifdef USE_MULTICORE
	EnterCriticalSection(& m_csFile);
#endif
	if (fputs(szFirstColumn, m_pFile) == EOF)
	{
		_snprintf_s(m_szErrorMsg, _TRUNCATE, "%s:%d Could not write to %s \n", 
					__FILE__, __LINE__, m_sFullFileName.c_str()); 
		sErrorMsg = m_szErrorMsg;

#ifdef USE_MULTICORE
		LeaveCriticalSection(& m_csFile);
#endif
		return false;
	}

	fputs(",", m_pFile);

	std::string sDate;
	for (std::vector<double>::const_iterator iter = values.begin(); iter != values.end(); iter++ )
	{
		double dValue = *iter;
					
		sprintf_s(m_szInputBuffer, "%f,", dValue);

		if (fputs(m_szInputBuffer, m_pFile) == EOF)
		{
			_snprintf_s(m_szErrorMsg, _TRUNCATE, "%s:%d Could not write to %s \n", 
						__FILE__, __LINE__, m_sFullFileName.c_str()); 
			sErrorMsg = m_szErrorMsg;

#ifdef USE_MULTICORE
			LeaveCriticalSection(& m_csFile);
#endif
			return false;
		}
	}

	fputs("\n", m_pFile);

#ifdef USE_MULTICORE
	LeaveCriticalSection(& m_csFile);
#endif
	return true;
}
 
