#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Most of the code in this class is lifted from the Tiberian Dawn profile
 * reading methods.
 */
class IniReader
{
private:
	static void __cdecl strtrim(char* buffer)
	{
	__asm {
		cmp[buffer], 0
		je	short fini

		; Prepare for string scanning by loading pointers.
		cld
		mov	esi, [buffer]
		mov	edi, esi

		; Strip white space from the start of the string.
		looper:
		lodsb
			cmp	al, 20h; Space
			je	short looper
			cmp	al, 9; TAB
			je	short looper
			stosb

			; Copy the rest of the string.
			gruntloop:
		lodsb
			stosb
			or al, al
			jnz	short gruntloop
			dec	edi
			; Strip the white space from the end of the string.
			looper2:
		mov[edi], al
			dec	edi
			mov	ah, [edi]
			cmp	ah, 20h
			je	short looper2
			cmp	ah, 9
			je	short looper2

			fini :
		//ret
	}
	}

	const char* iniText;

	IniReader(const char* iniText) : iniText(iniText)
	{
	}

public:
	static IniReader& Build(const char* iniText)
	{
		return *(new IniReader(iniText));
	}

	~IniReader()
	{
		delete iniText;
	}

	int ReadInt(char const* section, char const* entry, int def)
	{
		bool dummy = false;

		return ReadInt(section, entry, def, &dummy);
	}

	int ReadInt(char const* section, char const* entry, bool* valueFound)
	{
		return ReadInt(section, entry, 0, valueFound);
	}

	int ReadInt(char const* section, char const* entry, int def, bool* valueFound)
	{
		char buffer[16];

		sprintf(buffer, "%d", def);

		ReadString(section, entry, buffer, buffer, 15, valueFound);

		return(atoi(buffer));
	}

	const char* ReadString(char const* section, char const* entry, char const* def, char* retbuffer, int retlen)
	{
		bool dummy = false;

		return ReadString(section, entry, def, retbuffer, retlen, &dummy);
	}

	const char* ReadString(char const* section, char const* entry, char* retbuffer, int retlen, bool* valueFound)
	{
		return ReadString(section, entry, NULL, retbuffer, retlen, valueFound);
	}

	const char* ReadString(char const* section, char const* entry, char const* def, char* retbuffer, int retlen, bool* valueFound)
	{
		const char* workptr,		// Working pointer into iniText block.
			      * altworkptr;	    // Alternate work pointer.
		char sec[50];			    // Working section buffer.
		const char* retval;			// Start of section or entry pointer.
		const char* next;			// Pointer to start of next section (or EOF).
		char c, c2;				    // Working character values.
		int	len;				    // Working substring length value.
		int	entrylen;		        // Byte length of specified entry.
		char* orig_retbuf;	        // original retbuffer ptr

		/*
		**	Fill in the default value just in case the entry could not be found.
		*/
		if (retbuffer) {
			if (def) {
				strncpy(retbuffer, def, retlen);
			}
			retbuffer[retlen - 1] = '\0';
			orig_retbuf = retbuffer;
		}

		/*
		**	Make sure a iniText string was passed in
		*/
		if (!iniText || !section) {
			*valueFound = false;

			return(retbuffer);
		}

		/*
		**	Build section string to match file image.
		*/
		sprintf(sec, "[%s]", section);	// sec = section name including []'s
		strupr(sec);
		len = strlen(sec);			    // len = section name length, incl []'s

		/*
		**	Scan for a matching section
		*/
		retval = iniText;
		workptr = iniText;
		for (;;) {
			/*
			**	'workptr' = start of next section
			*/
			workptr = strchr(workptr, '[');

			/*
			**	If the end has been reached without finding the desired section
			**	then abort with a failure flag.
			*/
			if (!workptr) {
				*valueFound = false;

				return(NULL);
			}

			/*
			**	'c' = character just before the '['
			*/
			if (workptr == iniText) {
				c = '\n';
			}
			else {
				c = *(workptr - 1);
			}

			/*
			**	If this is the section name & the character before is a newline,
			**	process this section
			*/
			if (memicmp(workptr, sec, len) == 0 && (c == '\n')) {
				/*
				**	Skip work pointer to start of first valid entry.
				*/
				workptr += len;
				while (isspace(*workptr)) {
					workptr++;
				}

				/*
				**	If the section name is empty, we will have stepped onto the start
				**	of the next section name; inserting new entries here will leave
				**	a blank line between this section's name & 1st entry. So, check
				**	for 2 newlines in a row & step backward.
				*/
				if (workptr - iniText > 4) {
					if (*(workptr - 1) == '\n' && *(workptr - 3) == '\n')
						workptr -= 2;
				}

				/*
				**	'next = end of section or end of file.
				*/
				next = strchr(workptr, '[');
				for (;;) {
					if (next) {

						c = *(next - 1);

						/*
						**	If character before '[' is newline, this is the start of the
						**	next section
						*/
						if (c == '\n') {
							if (*(next - 1) == '\n' && *(next - 3) == '\n') {
								next -= 2;
							}
							break;
						}

						/*
						**	This bracket was in the section; keep looking
						*/
						next = strchr(next + 1, '[');
					}
					else {

						/*
						**	No bracket found; set 'next' to the end of the file
						*/
						next = workptr + strlen(workptr) - 1;
						break;
					}
				}

				/*
				**	If a specific entry was specified then return with the associated
				**	string.
				*/
				if (entry) {
					retval = workptr;
					entrylen = strlen(entry);

					for (;;) {
						/*
						** Search for the 1st character of the entry
						*/
						workptr = strchr(workptr, *entry);

						/*
						**	If the end of the file has been reached or we have spilled
						**	into the next section, then abort
						*/
						if (!workptr || workptr >= next) {
							*valueFound = false;

							return(NULL);
						}

						/*
						**	'c' = character before possible entry; must be a newline
						**	'c2' = character after possible entry; must be '=' or space
						*/
						c = *(workptr - 1);
						c2 = *(workptr + entrylen);

						/*
						**	Entry found; extract it
						*/
						if (memicmp(workptr, entry, entrylen) == 0 && (c == '\n') &&
							(c2 == '=' || isspace(c2))) {
							retval = workptr;
							workptr += entrylen;							// skip entry name
							workptr = strchr(workptr, '=');			// find '='

							/*
							** 'altworkptr' = next newline; \r is used here since we're
							** scanning forward!
							*/
							if (workptr) {
								altworkptr = strchr(workptr, '\r');	// find next newline
							}

							/*
							**	Return if there was no '=', or if the newline is before
							**	the next '='
							*/
							if (workptr == NULL || altworkptr < workptr) {
								*valueFound = false;

								return(retval);
							}

							/*
							**	Skip any white space after the '=' and before the first
							**	valid character of the parameter.
							*/
							workptr++;										// Skip the '='.
							while (isspace(*workptr)) {

								/*
								**	Just return if there's no entry past the '='.
								*/
								if (workptr >= altworkptr) {
									*valueFound = false;

									return(retval);
								}

								workptr++;									// Skip the whitespace
							}

							/*
							**	Copy the entry into the return buffer.
							*/
							len = (int)(altworkptr - workptr);
							if (len > retlen - 1) {
								len = retlen - 1;
							}

							if (retbuffer) {
								memcpy(retbuffer, workptr, len);
								*(retbuffer + len) = '\0';		// Insert trailing null.
								strtrim(retbuffer);
							}

							*valueFound = true;

							return(retval);
						}

						/*
						**	Entry was not found; go to the next one
						*/
						workptr++;
					}
				}
				else {
					/*
					**	No entry was specified, so build a list of all entries.
					**	'workptr' is at 1st entry after section name
					**	'next' is next bracket, or end of file
					*/
					retval = workptr;

					if (retbuffer) {

						/*
						**	Keep accumulating the identifier strings in the retbuffer.
						*/
						while (workptr && workptr < next) {
							altworkptr = strchr(workptr, '=');		// find '='

							if (altworkptr && altworkptr < next) {
								int	length;								// Length of ID string.

								length = (int)(altworkptr - workptr);

								/*
								**	Make sure we don't write past the end of the retbuffer;
								**	add '3' for the 3 NULL's at the end
								*/
								if (retbuffer - orig_retbuf + length + 3 < retlen) {
									memcpy(retbuffer, workptr, length);	// copy entry name
									*(retbuffer + length) = '\0';			// NULL-terminate it
									strtrim(retbuffer);						// trim spaces
									retbuffer += strlen(retbuffer) + 1;	// next pos in dest buf
								}
								else {
									break;
								}

								/*
								**	Advance the work pointer to the start of the next line
								**	by skipping the end of line character.
								*/
								workptr = strchr(altworkptr, '\n');
								if (!workptr) {
									break;
								}
								workptr++;
							}
							else {

								/*
								**	If no '=', break out of loop
								*/
								break;
							}
						}

						/*
						**	Final trailing terminator. Make double sure the double
						**	trailing null is added.
						*/
						*retbuffer++ = '\0';
						*retbuffer++ = '\0';
					}
					break;
				}
			}
			else {
				/*
				**	Section name not found; go to the next bracket & try again
				**	Advance past '[' and keep scanning.
				*/
				workptr++;
			}
		}

		*valueFound = false;

		return(retval);
	}
};
