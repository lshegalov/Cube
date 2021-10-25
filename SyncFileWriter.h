#pragma once


class SyncFileWriter
{
public:
	SyncFileWriter();
	SyncFileWriter(const SyncFileWriter & other);
	SyncFileWriter & operator= (const SyncFileWriter &rhs) = delete;

	~SyncFileWriter();

	bool createFile(__in const char * szOutputDirectory, 
		            __in const char * szOutputFileName, // with or without .csv or other extention
					__in const char * szExtention,      // ".csv", ".ok" ...
		            __out std::string & sErrorMsg);

	bool appendToFile(__in const char * szOutputDirectory, 
		              __in const char * szOutputFileName, // with or without .csv or other extention
					  __in const char * szExtention,      // ".csv", ".ok" ...
		              __out std::string & sErrorMsg);

	void closeFile();

	bool IsActive() const;
		
	const char * getFullFileName() const;

	bool printStrings(__in const char * szFirstColumn, const std::vector<std::string> & values,
		              __out std::string & sErrorMsg);
	bool printDoubles(__in const char * szFirstColumn, const std::vector<double> & values,
					  __out std::string & sErrorMsg);

	bool write(const char* szFormat,  // (I) printf style format string.
		       ...);                   // (I) Variable arguments. 

private:
	//void clearData();
	void copyFromOther(const SyncFileWriter & other);

	bool appendOrCreateFile(__in const char * szOutputDirectory, 
							__in const char * szOutputFileName, // with or without .csv or other extention
							__in const char * szExtention,      // ".csv", ".ok" ...
							__in bool bCreateNewFile,
		                    __out std::string & sErrorMsg);



	bool writeWithFormat(const char * szFormat,  va_list parminfo);

private:
	std::string m_sFullFileName;

	FILE * m_pFile;

	char m_szInputBuffer[32767];

	char m_szErrorMsg[ERROR_MSG_LEN];

#ifdef USE_MULTICORE
	CRITICAL_SECTION m_csFile;
#endif
};

