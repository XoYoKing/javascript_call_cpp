
// JsCallCppDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JsCallCpp.h"
#include "JsCallCppDlg.h"
#include "afxdialogex.h"
#include <MsHTML.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CJsCallCppDlg �Ի���



CJsCallCppDlg::CJsCallCppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJsCallCppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJsCallCppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_webbrowser);
}

BEGIN_MESSAGE_MAP(CJsCallCppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CJsCallCppDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CJsCallCppDlg ��Ϣ�������

BOOL CJsCallCppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//������Դ�ļ��е�HTML,IDR_HTML1����HTML�ļ�����Դ�ļ��е�ID
	wchar_t self_path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, self_path, MAX_PATH);
	CString res_url;
	res_url.Format(L"res://%s/%d", self_path, IDR_HTML1);
	m_webbrowser.Navigate(res_url, NULL, NULL, NULL, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CJsCallCppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CJsCallCppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//���Լ����ҵ����������ⶨ������ID�����ID����ȡ0-16384֮���������
enum
{
	FUNCTION_ShowMessageBox = 1,
	FUNCTION_GetProcessID = 2,
};

//����ʵ�֣�ֱ�ӷ���E_NOTIMPL
HRESULT STDMETHODCALLTYPE CJsCallCppDlg::GetTypeInfoCount(UINT *pctinfo)
{
	return E_NOTIMPL;
}

//����ʵ�֣�ֱ�ӷ���E_NOTIMPL
HRESULT STDMETHODCALLTYPE CJsCallCppDlg::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;
}

//JavaScript�����������ķ���ʱ����ѷ��������ŵ�rgszNames�У�������Ҫ������������ⶨһ��Ψһ������ID����rgDispId���ظ���
//ͬ��JavaScript��ȡ������������ʱ������������ŵ�rgszNames�У�������Ҫ������������ⶨһ��Ψһ������ID����rgDispId���ظ���
//������JavaScript�����Invoke���������ID��Ϊ�������ݽ���
HRESULT STDMETHODCALLTYPE CJsCallCppDlg::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	//rgszNames�Ǹ��ַ������飬cNamesָ������������м����ַ������������1���ַ�����������
	if (cNames != 1)
		return E_NOTIMPL;
	//����ַ�����ShowMessageBox��˵��JavaScript�ڵ�������������ShowMessageBox�������ҾͰ����ⶨ��IDͨ��rgDispId������
	if (wcscmp(rgszNames[0], L"ShowMessageBox") == 0)
	{
		*rgDispId = FUNCTION_ShowMessageBox;
		return S_OK;
	}
	//ͬ������ַ�����GetProcessID��˵��JavaScript�ڵ�������������GetProcessID����
	else if (wcscmp(rgszNames[0], L"GetProcessID") == 0)
	{
		*rgDispId = FUNCTION_GetProcessID;
		return S_OK;
	}
	else
		return E_NOTIMPL;
}

//JavaScriptͨ��GetIDsOfNames�õ��ҵĶ���ķ�����ID�󣬻����Invoke��dispIdMember���Ǹղ��Ҹ����������Լ��ⶨ��ID
//wFlagsָ��JavaScript���ҵĶ������ʲô���飡
//�����DISPATCH_METHOD��˵��JavaScript�ڵ����������ķ���������cpp_object.ShowMessageBox();
//�����DISPATCH_PROPERTYGET��˵��JavaScript�ڻ�ȡ�����������ԣ�����var n = cpp_object.num;
//�����DISPATCH_PROPERTYPUT��˵��JavaScript���޸������������ԣ�����cpp_object.num = 10;
//�����DISPATCH_PROPERTYPUTREF��˵��JavaScript��ͨ�������޸�������󣬾�����Ҳ����
//ʾ�����벢û���漰��wFlags�Ͷ������Ե�ʹ�ã���Ҫ���������о����÷���һ����
//pDispParams����JavaScript�����ҵĶ���ķ����Ǵ��ݽ����Ĳ�����������һ�����鱣�������в���
//pDispParams->cArgs�����������ж��ٸ�����
//pDispParams->rgvarg���Ǳ����Ų��������飬��ʹ��[]�±������ʣ�ÿ����������VARIANT���ͣ����Ա���������͵�ֵ
//������ʲô������VARIANT::vt���жϣ���������ˣ�VARIANT�ⶫ����Ҷ���
//pVarResult�������Ǹ�JavaScript�ķ���ֵ
//�������ù�
HRESULT STDMETHODCALLTYPE CJsCallCppDlg::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
	WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	//ͨ��ID�Ҿ�֪��JavaScript������ĸ�����
	if (dispIdMember == FUNCTION_ShowMessageBox)
	{
		//����Ƿ�ֻ��һ������
		if (pDispParams->cArgs != 1)
			return E_NOTIMPL;
		//�����������Ƿ����ַ�������
		if (pDispParams->rgvarg[0].vt != VT_BSTR)
			return E_NOTIMPL;
		//���ĵ���
		ShowMessageBox(pDispParams->rgvarg[0].bstrVal);
		return S_OK;
	}
	else if (dispIdMember == FUNCTION_GetProcessID)
	{
		DWORD id = GetProcessID();
		*pVarResult = CComVariant(id);
		return S_OK;
	}
	else
		return E_NOTIMPL;

}


//JavaScript�õ����Ǵ��ݸ�����ָ�����������������ǵĶ�����ʲô�����������QueryInterface��ѯ�����ǡ�����ʲô��������
//����ͨ��riid����������ʲô������ֻ�����ʵ������ǲ���IID_IDispatch�������ǲ���IID_IUnknownʱ�����ǲ��ܿ϶��Ļش���S_OK
//��Ϊ���ǵĶ���̳���IDispatch����IDispatch�ּ̳���IUnknown������ֻʵ�����������ӿڣ�����ֻ���������ش�����ѯ��
HRESULT STDMETHODCALLTYPE CJsCallCppDlg::QueryInterface(REFIID riid, void **ppvObject)
{
	if (riid == IID_IDispatch || riid == IID_IUnknown)
	{
		//�Եģ�����һ��IDispatch�������Լ�(this)������
		*ppvObject = static_cast<IDispatch*>(this);
		return S_OK;
	}
	else
		return E_NOINTERFACE;
}

//����֪��COM����ʹ�����ü�������������������ڣ����ǵ�CJsCallCppDlg������������ھ��������������������
//�ҵ����������Ҫ��JavaScript���ܣ����Լ���ܣ������Ҳ���ʵ��AddRef()��Release()��������дһЩ��
//��Ҫreturn 1;return 2;return 3;return 4;return 5;������
ULONG STDMETHODCALLTYPE CJsCallCppDlg::AddRef()
{
	return 1;
}

//ͬ�ϣ�����˵��
//���⻰����Ȼ�����Ҫnew��һ��c++���������Ӹ�JavaScript���ܣ������Ҫʵ��AddRef()��Release()�������ü�������ʱdelete this;
ULONG STDMETHODCALLTYPE CJsCallCppDlg::Release()
{
	return 1;
}

//����JavaScript��SaveCppObject�����������Լ�(this)��������SaveCppObject�����������󱣴浽ȫ�ֱ���var cpp_object;��
//�Ժ�JavaScript�Ϳ���ͨ��cpp_object�����������C++����ķ�����
void CJsCallCppDlg::OnBnClickedOk()
{
	CComQIPtr<IHTMLDocument2> document = m_webbrowser.get_Document();
	CComDispatchDriver script;
	document->get_Script(&script);
	CComVariant var(static_cast<IDispatch*>(this));
	script.Invoke1(L"SaveCppObject", &var);
}

DWORD CJsCallCppDlg::GetProcessID()
{
	return GetCurrentProcessId();
}

void CJsCallCppDlg::ShowMessageBox(const wchar_t *msg)
{
	MessageBox(msg, L"��������javascript����Ϣ");
}

