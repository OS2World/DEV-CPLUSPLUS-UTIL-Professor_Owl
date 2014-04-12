
class ProjFile
{
private:
    char FileName[MAXPATHSIZE]; // Path/Name of Project.owl file
    FILE *Fi;                   // File Pointer
public:
    ProjFile(void);             // open PROJECT.OWL
    ~ProjFile();                // close it
    int  IsEof(void);           // test for end of file
    void Rewind(void);          // rewind to string #1
    void GetFile(char *s);      // 256 char string - Get a Path\Filename
    void Close(void);           // close it!
    void AppendString(char *add1, char *add2);  // Add String To End of file
private:
    void getrawstring(char *s); // get string w/o processing
};


void PrettyPathFile(char *s);

