#pragma once


class SyncFileWriter
{
public:
	SyncFileWriter();
	SyncFileWriter(const SyncFileWriter & other);
	SyncFileWriter & operator= (const SyncFileWriter &rhs) = delete;

	~SyncFileWriter();

	bool createFile(IN const char * szOutputDirectory, 
		            IN const char * szOutputFileName, // with or without .csv or other extention
					IN const char * szExtention,      // ".csv", ".ok" ...
		            OUT std::string & sErrorMsg);

	bool appendToFile(IN const char * szOutputDirectory, 
		              IN const char * szOutputFileName, // with or without .csv or other extention
					  IN const char * szExtention,      // ".csv", ".ok" ...
		              OUT std::string & sErrorMsg);

	void closeFile();

	bool IsActive() const;
		
	const char * getFullFileName() const;

	bool printStrings(IN const char * szFirstColumn, const std::vector<std::string> & values,
		              OUT std::string & sErrorMsg);
	bool printDoubles(IN const char * szFirstColumn, const std::vector<double> & values,
					  OUT std::string & sErrorMsg);

	bool write(const char* szFormat,  // (I) printf style format string.
		       ...);                   // (I) Variable arguments. 

private:
	//void clearData();
	void copyFromOther(const SyncFileWriter & other);

	bool appendOrCreateFile(IN const char * szOutputDirectory, 
							IN const char * szOutputFileName, // with or without .csv or other extention
							IN const char * szExtention,      // ".csv", ".ok" ...
							IN bool bCreateNewFile,
		                    OUT std::string & sErrorMsg);



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

