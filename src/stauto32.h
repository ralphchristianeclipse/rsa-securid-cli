#ifndef STAUTO32_H
#define STAUTO32_H

/*! \mainpage

<H1>RSA SecurID Software Token 4.1.1 Developer's Guide</H1>

<p>The RSA SecurID Software Token 4.1.1 application programming interface (API) allows
third-party vendors to gain access to software token metadata and tokencodes. Any
program that can use Windows DLLs or Apple Frameworks that export C-style functions
can use the RSA SecurID Software Token application's library and retrieve passcodes directly.
Programs using the API can be written in C++, Visual Basic, or C#, as well as other languages.</p>

<H2>Supported Environments</H2>
<ul>
    <li>Windows XP Professional SP3</li>
    <li>Windows Server 2003 Enterprise Edition</li>
    <li>Windows Vista Business SP1/SP2</li>
    <li>Windows Vista Enterprise SP1/SP2</li>
    <li>Windows Server 2008 Enterprise Edition</li>
    <li>Windows 7 Professional</li>
    <li>Windows 7 Enterprise</li>
    <li>Mac OS X v10.5 (Leopard)</li>
</ul>

<H2>Installing the RSA SecurID Software Token 4.1.1 API</H2>
<H3>On Windows:</H3>
<p>The RSA SecurID Software Token 4.1.1 API DLL (<b>stauto32.dll</b>) is automatically installed
with the RSA SecurID Software Token with Automation 4.1.1 application or with the included merge module.</p>
<p>The library install directory ("C:/Program Files/RSA SecurID Token Common") can be
found by looking in the Windows registry at the "InstallDir" value in the following key:
HKEY_LOCAL_MACHINE/SOFTWARE/RSA/Software Token/Library.</p>
<H3>On Mac:</H3>
<p>The RSA SecurID Software Token 4.1 framework (<b>stauto32.framework</b>) is automatically installed
with the RSA SecurID Software Token 4.1 application.</p>
<p>The framework is installed at "/Library/Frameworks".</p>

<H2>Upgrading Previous Versions</H2>
<p>The RSA SecurID Software Token 4.1.1 installation automatically upgrades earlier 3.0, 4.0 or 4.1 versions of the API.
On Windows, if the API components are installed with the merge module, previous versions may not be upgraded.
If you use the merge module, the user may need to uninstall previous versions.</p>

<H2>Development Instructions</H2>
<H3>On Windows:</H3>
<p>If you do not want to create a dependency on the stauto32 DLL, RSA recommends dynamically loading the DLL
at runtime if it exists on the user's system. Use the Windows API function "LoadLibraryEx" to load
the DLL, use "GetProcAddress" to resolve the symbols, and use "FreeLibrary" to unload the DLL.
To have the symbols from stauto32 loaded implicitly, add <b>stauto32.lib</b> (included with the SDK package)
to your project as a dependency. Be aware that your binary will not load without either the RSA SecurID Software
Token 4.1.1 application or the merge module installed on the user's system.</p>
<p>The stauto32 library depends on Qt libraries. To properly point to these Qt
libraries on Windows, retrieve the required path information from the "InstallDir" registry value
located in HKEY_LOCAL_MACHINE/SOFTWARE/RSA/Software Token/Library, and use that path information
with "LoadLibraryEx" and the flag "LOAD_WITH_ALTERED_SEARCH_PATH" to load <b>stauto32.dll</b>.</p>
<H3>On Mac:</H3>
<p>If you do not want to create a dependency on the stauto32 framework, RSA recommends dynamically loading the framework
at runtime if it exists on the user's system. Use the UNIX API function "dlopen" to load the framework
(for example: dlopen("stauto32.framework/stauto32", RTLD_LAZY)). Use "dlsym" to resolve the symbols, and use "dlclose"
to unload the framework. To have the symbols from stauto32 loaded implicitly, install the RSA SecurID
Software Token 4.1 application locally, and add <b>stauto32.framework</b>, located in /Library/Frameworks, to your project
as a dependency. Be aware that if you add the stauto32 framework to your project as a dependency, your binary will not
load unless the RSA SecurID Software Token 4.1 application has been installed on the user's system.</p>

<H2>RSA SecurID Software Token 4.1.1 API Notes</H2>
<ul>
<li>The RSA SecurID Software Token 4.1.1 API supports multiple instances of the token service
and can handle multiple token devices.</li>
<li>To allow your program to include multiple instances of the token service, a handle has been implemented to
identify each instance. Your program must call OpenTokenService() and store the handle
that is returned. Your program must use this handle in all future calls to the API and call
CloseTokenService() to close the instance of the API.</li>
<li>The API supports multiple tokens. The EnumToken() function enumerates all of the
available tokens provided by the token service. After the tokens have been enumerated,
the program can either use the default token already selected or call SelectToken() to
change the current token.</li>
<li>If the production "Local Hard Drive (RSA)" plug-in is loaded, you will not be able to attach a debugger.</li>
<li>Because copying software tokens from one computer to another computer can breach security,
the software tokens are locked to the device on which they are installed.
You cannot transfer a token between devices.</li>
</ul>

<hr>

<H1>Using the API</H1>
<p>The API is typically used as follows:</p>
<table cellpadding="2" cellspacing="2">
    <tr>
        <th>Function</th>
        <th>Application Behavior</th>
    </tr>
    <tr>
        <td valign="top"><b>OpenTokenService()</b></td>
        <td valign="top">Open an instance of the token service and get a handle to it.</td>
    </tr>
    <tr>
        <td valign="top"><b>EnumToken()</b></td>
        <td valign="top">List the available tokens, present the list to the user, and allow the user to select which one to use.</td>
    </tr>
    <tr>
        <td valign="top"><b>GetCurrentCode()</b></td>
        <td valign="top">Ask the user for a PIN and use it to retrieve a passcode in the following scenario:
            <ul>
                <li>Standard Passcode Mode -- Send passcode to authenticate.</li>
            </ul>
        </td>
    </tr>
    <tr>
        <td valign="top"><b>GetNextCode()</b></td>
        <td valign="top">Use this function in the following scenarios:
            <ul>
                <li>Next Tokencode Mode -- After sending passcode (from GetCurrentCode()),
                obtain the next tokencode and send it to authenticate.</li>
                <li>New PIN Mode -- Send tokencode (from GetCurrentCode()), retrieve PIN from user
                and use it with this function, then send next passcode.</li>
            </ul>
        </td>
    </tr>
    <tr>
        <td valign="top"><b>CloseTokenService()</b></td>
        <td valign="top">Close the token service.</td>
    </tr>
</table>
<p>When using PINs with GetCurrentCode() and GetNextCode(), the passcode returned will vary based
on the type of token. There are three different types:
<ul>
    <li>PINPad-style - With this type of token, the numeric PIN is mixed or rolled into the tokencode. The
    passcode will be the same length as the tokencode (typically 6 or 8 digits).</li>
    <li>Fob-style - With this type of token, the alphanumeric PIN is prepended to the tokencode which
    can result in a passcode with a maximum length of 16 (8 characters for the PIN and 8 characters for the
    tokencode).</li>
    <li>PINless - With this type of token, the PIN is ignored if it is passed in, and the passcode will
    always be the same as the tokencode.</li>
</ul></p>

<hr>

<H1>Changes Since RSA SecurID Token 3.0.x</H1>
<H2>Device Binding and Policies</H2>
<p>Tokens may be bound to a specific device, by serial number or device type. Stauto32 enforces
these bindings. If a token is bound to a device and the device is not present, the token cannot be imported.</p>
<p>The stauto32 library also honors policies set using GPO policy. One example is the device whitelist,
which restricts the possible token storage devices to which a user can import tokens. For other policies,
see the <i>RSA SecurID Software Token 4.1.1 Administrator's Guide</i>.</p>
<H2>Time Providers</H2>
<p>The SetTokenTime() function is no longer supported due to security issues. The ability to set the token time
through an API function has been replaced by the ability to set a time provider plug-in. When a time provider
plug-in approved by RSA is installed, registered, and set, it provides the time used to calculate tokencodes
on token storage devices that support it. For information on developing a time provider plug-in, contact
RSA Partner Engineering. For more details on setting the time provider plug-in of your choice, see
SetTimeProvider().</p>
<H2>Changes to the Stauto32 API</H2>
<ul>
<li>Best practice is now to call OpenTokenService() only once per session. Opening and closing multiple times
causes performance issues and may lead to multiple password prompts.</li>
<li>All strings returned are UTF-8 encoded.</li>
<li>Per-token passwords are no longer supported. Passwords are now per-device. A "device" is a logical
storage container for tokens. It may be the user's hard disk, a TPM, a smart-card, a flash drive, or another supported device.</li>
</ul>
<H3>Functions That Have Been Removed</H3>
<table cellpadding="2" cellspacing="2">
    <tr>
        <th>Function</th>
        <th>Alternative</th>
    </tr>
    <tr>
        <td valign="top">StartSoftID()</td>
        <td valign="top">OpenTokenService()</td>
    </tr>
    <tr>
        <td valign="top">StopSoftID()</td>
        <td valign="top">CloseTokenService()</td>
    </tr>
    <tr>
        <td valign="top">SetPRNInfo()</td>
    </tr>
    <tr>
        <td valign="top">GetPRNInfo()</td>
        <td valign="top">GetCurrentCode() or GetNextCode()</td>
    </tr>
    <tr>
        <td valign="top">AddPINToPRN()</td>
    </tr>
    <tr>
        <td valign="top">GetSelectTokenInfo()</td>
        <td valign="top">SelectToken()</td>
    </tr>
    <tr>
        <td valign="top">SetEnumTokenInfo()</td>
        <td valign="top">EnumToken()</td>
    </tr>
    <tr>
        <td valign="top">GetPasscode()</td>
        <td valign="top">GetCurrentCode() or GetNextCode()</td>
    </tr>
    <tr>
        <td valign="top">GetDLLInfo()</td>
    </tr>
    <tr>
        <td valign="top">GetDeviceInfo()</td>
    </tr>
    <tr>
        <td valign="top">NewPinMode()</td>
    </tr>
    <tr>
        <td valign="top">SetTokenTime()</td>
        <td valign="top">SetTimeProvider()</td>
    </tr>
    <tr>
        <td valign="top">ConfigToken()</td>
    </tr>
</table>

<hr>

<H1>Recommended Field Labels</H1>
<H2>Scenario: Application Is Configured To Recognize a Software Token</H2>
<H3>Use case 1: Token Requires a PIN</H3>
This applies to both fob-style and PINPad-style tokens.
<ul>
<li>Enter user name:</li>
<li>Enter PIN:</li>
</ul>
<H3>Use case 2: PINless Token</H3>
<ul>
<li>Enter user name:</li>
</ul>
<H2>Scenario: Application Is Not Configured To Recognize a Software Token</H2>
<H3>Use Case 1: PINPad-style Token</H3>
<ul>
<li>Enter user name:</li>
<li>Enter passcode:</li>
</ul>
<H3>Use Case 2: Fob-style Token</H3>
<ul>
<li>Enter user name:</li>
<li>Enter PIN:</li>
<li>Enter tokencode:</li>
</ul>
<H3>Use Case 3: PINless Token</H3>
<ul>
<li>Enter user name:</li>
<li>Enter tokencode:</li>
</ul>

<HR>

    COPYRIGHT (C) 2009 RSA SECURITY INC.  ALL RIGHTS RESERVED.

    THIS SOFTWARE IS PROPRIETARY AND CONFIDENTIAL TO RSA SECURITY INC.,
    IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED
    ONLY IN ACCORDANCE THE TERMS OF SUCH LICENSE AND WITH THE INCLUSION
    OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR ANY OTHER COPIES THEREOF
    MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY OTHER PERSON.  NO
    TITLE TO AND OWNERSHIP OF THE SOFTWARE IS HEREBY TRANSFERRED.

    THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
    SHOULD NOT BE CONSTRUED AS A COMMITMENT BY RSA SECURITY INC.

*/

#ifndef _SOFTAPI_H
#define _SOFTAPI_H
#if defined(__APPLE__) || defined(applec)
	typedef	long	DWORD;
	typedef	int		INT;
	typedef	int     BOOL;
	typedef BOOL*	LPBOOL;
	typedef	long	LONG;
	typedef	char*	LPSTR;
	typedef	const char*	LPCSTR;
	typedef	long*	LPLONG;
	typedef	void*	LPVOID;
	typedef	long*	LPDWORD;

	#ifdef STAUTO32_EXPORT
	   #define SDIAPI  __attribute__((visibility("default")))
	#else
	   #define SDIAPI
	#endif

	#define	CDECL_SPEC
#else
	#ifndef _WINDOWS_
		#include <windows.h>
	#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifdef _WINDOWS_
	#ifdef STAUTO32_EXPORT
	   #define SDIAPI  __declspec(dllexport) WINAPI
	#else
	   #define SDIAPI  __declspec(dllimport) WINAPI
	#endif
	#define CDECL_SPEC WINAPI
#endif

#ifndef CDECL_SPEC
	#define CDECL_SPEC __stdcall
#endif


/* STAUTO32_STATIC: Builds a static version of the Token library used for internal RSA projects.
   Third parties should not define this. */
#if defined(STAUTO32_STATIC)
	#define STAUTO32_API_INT  INT
	#define STAUTO32_API_BOOL BOOL
	#define STAUTO32_API_LONG LONG
#else
	#define STAUTO32_API_INT  INT  SDIAPI
	#define STAUTO32_API_BOOL BOOL SDIAPI
	#define STAUTO32_API_LONG LONG SDIAPI
#endif

#ifndef	TRUE
	#define TRUE            1
#endif
#ifndef	FALSE
	#define FALSE           0
#endif



///////////////////////////////////////////////////////////////////////////////
// Error Codes

/*! Display Error */
#define ERROR_DISPLAY_STRING           -1
/*! No Error Reported */
#define ERROR_NONE                      0
/*! General Error */
#define ERROR_INIT                      1
#define ERROR_INVALID_PNTR              1
/*! Time Error */
#define ERROR_TIME                      2
#define ERROR_DLL_LOADED                2
#define ERROR_SOFTID_CANNOTRUN          2
#define ERROR_LOAD_DLL_ERROR            3
/*! Security Error - Error may be due to invalid use of a restricted function */
#define ERROR_SECURITY                  3
#define ERROR_SOFTID_DEAD               3
/*! Resource Error */
#define ERROR_RESOURCE_ERROR            4
/*! Registry Read Error */
#define ERROR_READ_REG_ERROR            5
/*! Registry Write Error */
#define ERROR_WRITE_REG_ERROR           6
/*! Invalid Token File */
#define ERROR_INVALID_SET_NUMBER        7
/*! Invalid Address of Exported Function */
#define ERROR_PROC_ADDRESS_ERROR        8
/*! Invalid Name */
#define ERROR_INVALID_NAME_ERROR        9
/*! DLL Error */
#define ERROR_DLL_CALL_ERROR            10
/*! Inactive DLL Error */
#define ERROR_INACTIVE_DLL              11
/*! Invalid Data Error - Error may be due to invalid function parameters */
#define ERROR_DATA_INVALID              12
/*! Token File Error */
#define ERROR_HANDLE_INVALID            13
/*! Invalid Algorithm - Tokencode calculation algorithm not supported on device */
#define ERROR_KEY_INVALID               14
/*! Invalid Token */
#define ERROR_TOKEN_INVALID             15
/*! Token is Unavailable */
#define ERROR_TOKEN_NOTFOUND            16
/*! Invalid Buffer Size - Parameter buffer size is too small */
#define ERROR_BUFFER_TOO_SMALL_ERROR    17
/*! Token Service Already Open - Can only occur with deprecated functions */
#define ERROR_TOKEN_SERVICE_OPEN        18
/*! Invalid Component Name */
#define ERROR_INVALID_COMP_NAME         19
/*! Current Token Deleted */
#define ERROR_SET_ONE_DELETED           20
/*! Incorrect Token Passphrase */
#define ERROR_PASSWORD_INVALID          21
/*! Incorrect Device Password */
#define ERROR_NONE_MATCHING_PASSWORD    22
/*! Device Copy Protection Mismatch */
#define ERROR_COPY_PROTECTION_DEVICE    23
/*! Token Copy Protection Mismatch */
#define ERROR_COPY_PROTECTION_TOKEN     24
/*! Error Generating Tokencode */
#define ERROR_TOKENCODE_GENERATION      25
/*! Token Database Error */
#define ERROR_DATABASE                  26
/*! DLL Not Found */
#define ERROR_LOAD_LIBRARY              27
/*! File Not Found - Unable to open file, possibly due to an invalid path */
#define ERROR_OPEN_FILE                 28
/*! Token Database Not Found */
#define ERROR_OPEN_DATABASE             29
/*! Smart Card Error */
#define ERROR_SCARD                     30
/*! PKCS#11 Error */
#define ERROR_P_ELEVEN                  31
/*! RSA Token Provider Plugin Error */
#define ERROR_PLUGIN                    32
/*! SDD API Error */
#define ERROR_SDD                       33
/*! PSD Error */
#define ERROR_PSD                       34
/*! Device Protected Session Required */
#define ERROR_DEVICE_LOGIN              35
/*! Select Token(s) to Import */
#define ERROR_SELECT_TOKEN_SERIAL       36
/*! Device Binding Error - Token is bound to a different device than the one selected */
#define ERROR_DEVICE_BINDING            37
/*! Invalid GUID Format - GUID should be DCE variant */
#define ERROR_GUID_INVALID              38
/*! Device Operation Not Supported */
#define ERROR_PLUGIN_IMPLEMENTATION     39
/*! Invalid Token File */
#define ERROR_FILE_FORMAT               40
/*! Communication Error - Error communicating with server */
#define ERROR_INET_COMMUNICATION        41
/*! Invalid Token File Password */
#define ERROR_TOKENFILE_PASSWORD_CHECK  42
/*! CT-KIP Failure - Server sent bad packet indicating CT-KIP error */
#define ERROR_SERVER_BAD_STATUS         43
/*! Duplicate Serial Number(s) - Import failed, token serial number already in use */
#define ERROR_DUPLICATE_SERIAL          44
/*! Untrusted SSL Certificate */
#define ERROR_SSL_UNTRUSTED_CERT        45
/*! Device Full - Maximum number of tokens already stored on device */
#define ERROR_DEVICE_FULL               46
/*! Device Reset */
#define ERROR_DEVICE_RESET              47
/*! Expiration Error - Import failed, token is expired */
#define ERROR_EXPIRED_TOKEN				48
/*! Unable to Set Device Password */
#define ERROR_PASSWORD_FAILURE	        216
/*! Token Migration Failure */
#define ERROR_IMPORT_FAILURE		    217




///////////////////////////////////////////////////////////////////////////////
// Data Structures

/*! \brief Structure containing token data. Used by EnumToken(). */
typedef struct tagTOKENBASICINFO
{
	DWORD    dwSize;            /*!< Size of struct in bytes */
	char     serialnumber[24];  /*!< Token serial number */
	char     username[24];      /*!< Username of user the token is assigned to */
	char     deviceID[24];      /*!< No longer used */
	char     descriptor[48];    /*!< No longer used */
} TOKENBASICINFO, * LPTOKENBASICINFO;

/*! \brief Structure containing error information. Used by GetTokenError(). */
typedef struct tagTOKENERRORINFO
{
	int error;                          /*!< Appropriate error code */
	char error_string[24];              /*!< Error information */
	char detailed_error_string[64];     /*!< Detailed error information */

} TOKENERRORINFO, * LPTOKENERRORINFO;

#ifndef CK_PTR
/* These definitions come from the PKCS #11 specification and are
   defined here so that you do not have to include the P11 headers. */

/*! An unsigned 8-bit value */
typedef unsigned char     CK_BYTE;

/*! An unsigned 8-bit character */
typedef CK_BYTE           CK_CHAR;

/*! An 8-bit UTF-8 character */
typedef CK_BYTE           CK_UTF8CHAR;

/*!
*  \brief CK_DATE is a structure that defines a date. It is the same
*   as CK_DATE, as defined in the PKCS #11 specification. Used by GetTokenExpirationDate().
*  \warning None of the member variables are null-terminated strings.
*/
struct CK_DATE
{
    CK_CHAR       year[4];   /*!< The year ("1900" - "9999") */
    CK_CHAR       month[2];  /*!< The month ("01" - "12") */
    CK_CHAR       day[2];    /*!< The day ("01" - "31") */
};
#define CK_PTR *
#endif



///////////////////////////////////////////////////////////////////////////////
// RSA SecurID Software Token API Functions


/*!
*   This function is called to gain access to the other API functions. It starts and initializes
*   the token service, and returns a service handle.
*
*  \param[out] pServiceHandle Pointer to a LONG that will receive the handle of the token service
*   started if the function succeeds. This handle must be used in all future calls to the token service.
*   If another program or process is using the DLL, it will be assigned its own handle that will
*   allow parallel use of the token service. When finished using the token service, CloseTokenService()
*   must be called.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT OpenTokenService (LPLONG pServiceHandle);

/*!
*   This function enumerates all tokens found in the open token service. It populates an array
*   of TOKENBASICINFO structures.
*
*   When pBuffer is NULL, pBufferSize is populated with the number of bytes needed to allocate
*   and the value FALSE is returned.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[out] pNumberOfTokens Pointer to a LONG that will receive the number of tokens found.
*
*  \param[out] pDefaultToken Pointer to a LONG that will receive the zero-based index of the default token in the array.
*
*  \param[out] pBuffer Pointer to a buffer that will receive the enumerated tokens. This buffer must
*   be a pointer to an array of TOKENBASICINFO structures.
*
*  \param[in,out]  pBufferSize Pointer to a DWORD that contains the number of bytes allocated for pBuffer.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT EnumToken (LONG ServiceHandle,
                            LPLONG pNumberOfTokens,
                            LPLONG pDefaultToken,
                            LPVOID  pBuffer,
                            LPDWORD pBufferSize);

/*!
*   If any API function fails, this function should be called to determine what error occurred.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[out] pErrorInfo Pointer to a TOKENERRORINFO structure that will receive information about
*   the last error that occurred in the token service.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT GetTokenError (LONG ServiceHandle,
                                LPTOKENERRORINFO pErrorInfo);

/*!
*   This function changes the current/default token to the token matching the serial
*   number passed in. If any deprecated functions that use a token are called (for example, GetPasscode),
*   the selected token is used.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[in] pSerialNumber Pointer to a constant null-terminated string that contains the serial
*   number of the token that will become the current/default token.
*
*  \param[in] pDeviceID No longer used. Applications may pass NULL for this parameter.
*
*  \param[in] pPassword No longer used. Applications may pass NULL for this parameter.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT SelectToken (LONG ServiceHandle,
                              LPCSTR pSerialNumber,
                              LPCSTR pDeviceID,
                              LPCSTR pPassword);

/*!
*   This function returns the token's expiration date as a CK_DATE.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[in] pSerialNumber Pointer to a constant null-terminated string that contains the serial
*   number of the token whose expiration date will be retrieved.
*
*  \param[out] pTokenExpirationInfo Pointer to a CK_DATE object that will receive the token's expiration
*   information.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT GetTokenExpirationDate (LONG ServiceHandle,
                                         LPCSTR pSerialNumber,
                                         CK_DATE* pTokenExpirationInfo);

/*!
*   This function should be called to specify which time provider plug-in to use. If possible,
*   the time provider chosen will be used as the time source for tokencode generation for the rest
*   of the token service session.
*
*   Setting the time provider will succeed even if the time provider plug-in isn't registered
*   or is unable to load as long as pGuid is a valid DCE variant UUID. In those cases, a default
*   system time provider will be used instead. Additionally, if a tokencode is being generated
*   for a particular token that has a time provider specified in its attributes, the token's time
*   provider will be given preference.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[in] pGuid Pointer to a constant null-terminated string that contains the
*   GUID (globally unique identifier) of a registered time provider dynamic library.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT SetTimeProvider (LONG ServiceHandle,
                                  LPCSTR pGuid);

/*!
*   This function determines whether the next tokencode can be obtained immediately
*   without blocking for a specified token.
*
*   Before retrieving the codes, this function calls SelectToken() for the token serial
*   number specified.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[in] pSerialNumber Pointer to a constant null-terminated string that contains the serial
*   number of the token the function will use.
*
*  \param[out] pbCanTokenGetNextCode Pointer to a BOOL that will receive the value TRUE
*   if the next tokencode can be obtained immediately, and FALSE if not.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT CanTokenGetNextCode (LONG ServiceHandle,
                                      LPCSTR pSerialNumber,
                                      LPBOOL pbCanTokenGetNextCode);

/*!
*   This function closes the token service.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT CloseTokenService (LONG ServiceHandle);

/*!
*   This function is used retrieve the current passcode and tokencode (PRN) from a specified
*   token. You are advised to use this function instead of the deprecated function GetPasscode(),
*   because it will not incur the performance penalty for retrieving the next code.
*
*   Before retrieving the codes, this function calls SelectToken() for the token serial
*   number specified.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[in] pSerialNumber Pointer to a constant null-terminated string that contains the serial
*   number of the token for which the code will be obtained. It will become the current/default token.
*
*  \param[in] pPin Pointer to a constant null-terminated string that contains the PIN to be used for
*   generating the passcode. If this parameter is set to NULL, an empty string, or four zeros (i.e. "0000"),
*   pPasscode will return a tokencode (PRN) instead.
*
*  \param[out] pTimeLeft The time left until the current code expires (-1 if unknown).
*
*  \param[out] pPasscode Pointer to a string that will receive the passcode (PIN plus tokencode).
*   If pPin is NULL or an empty string, pPasscode will return the tokencode.
*
*  \param[out] pPRN Pointer to a string that will receive the tokencode (PRN).
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT GetCurrentCode(LONG ServiceHandle,
                                LPCSTR pSerialNumber,
                                LPCSTR pPin,
                                LPLONG pTimeLeft,
                                LPSTR pPasscode,
                                LPSTR pPRN);

/*!
*   This function is used retrieve the next passcode or tokencode (PRN) from a specified
*   token. You are advised to use this function instead of GetPasscode() because it does not
*   incur the performance penalty for retrieving the current code. This function may block
*   up to one token interval for tokens that do not support getting the next tokencode immediately.
*
*   Before retrieving the codes, this function calls SelectToken() for the token serial
*   number specified.
*
*  \param[in] ServiceHandle Handle that identifies the instance of the token service.
*
*  \param[in] pSerialNumber Pointer to a constant null-terminated string that contains the serial
*   number of the token for which the code will be obtained. It will become the current/default token.
*
*  \param[in] pPin Pointer to a constant null-terminated string that contains the PIN to be used for
*   generating the next passcode. If this parameter is set to NULL, an empty string, or four zeros (i.e. "0000"),
*   pPasscode will return a tokencode (PRN) instead.
*
*  \param[out] pTimeLeft The time left until the next code expires (-1 if unknown).
*
*  \param[out] pPasscode Pointer to a string that will receive the next passcode (PIN plus tokencode).
*   If pPin is NULL or an empty string, pPasscode will return the next tokencode.
*
*  \param[out] pPRN Pointer to a string that will receive the next tokencode.
*
*  \return If the function succeeds, the return value is greater than zero.
*   If the function fails, the return value is zero.
*/
STAUTO32_API_INT GetNextCode(LONG ServiceHandle,
                             LPCSTR pSerialNumber,
                             LPCSTR pPin,
                             LPLONG pTimeLeft,
                             LPSTR pPasscode,
                             LPSTR pPRN);


///////////////////////////////////////////////////////////////////////////////////////

/* If the Token API is being used for internal RSA projects, include the
    extended Token API functionality. Third parties should not have access to this functionality. */
#ifdef _RSA_INTERNAL
#include "stauto32ext.h"
#endif

/*! \cond */
//////////////////////////////////////////////////////////////////////////////
// CONVENIENCE TYPEDEFS (for use when casting values from GetProcAddress() or dlsym())

typedef int  (CDECL_SPEC *OPENTOKENSERVICE)         (LPLONG);
typedef int  (CDECL_SPEC *ENUMTOKEN)                (LONG, LPLONG, LPLONG, LPVOID, LPDWORD);
typedef int  (CDECL_SPEC *GETTOKENERROR)            (LONG, LPTOKENERRORINFO);
typedef int  (CDECL_SPEC *CLOSETOKENSERVICE)        (LONG);
typedef int  (CDECL_SPEC *SELECTTOKEN)              (LONG, LPCSTR, LPCSTR, LPCSTR);
typedef int  (CDECL_SPEC *GETCURRENTCODE)           (LONG, LPCSTR, LPCSTR, LPLONG, LPSTR, LPSTR);
typedef int  (CDECL_SPEC *GETNEXTCODE)              (LONG, LPCSTR, LPCSTR, LPLONG, LPSTR, LPSTR);
typedef int  (CDECL_SPEC *CANTOKENGETNEXTCODE)      (LONG, LPCSTR, LPBOOL);
typedef int  (CDECL_SPEC *GETTOKENEXPIRATIONDATE)   (LONG, LPCSTR, CK_DATE*);
typedef int  (CDECL_SPEC *SETTIMEPROVIDER)          (LONG, LPCSTR);

/*! \endcond */

#ifdef __cplusplus
}
#endif

#endif

#endif // STAUTO32_H
