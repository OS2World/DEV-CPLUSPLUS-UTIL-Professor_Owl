//
// Created Mon Feb 20 20:51:50 1995
//
class About : public TDialog
{
private:    // Variables
    TStatic *m_HomeDir;
	TStatic *m_Version;
public:    // Methods
     About(TWindow *parent);
     virtual ~About();
        BOOL virtual EvInitDialog(HWND hWndFocus);
        void EvClickCancel(void);
        void EvClickOk(void);
    DECLARE_RESPONSE_TABLE(About);
protected:    // Events
}; // end of class About


