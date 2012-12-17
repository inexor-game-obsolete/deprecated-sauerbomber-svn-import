#import <Cocoa/Cocoa.h>

#define MAXSTRLEN 260
inline char *s_strncpy(char *d, const char *s, size_t m) { strncpy(d,s,m); d[m-1] = 0; return d; };
inline char *s_strcat(char *d, const char *s) { size_t n = strlen(d); return s_strncpy(d+n,s,MAXSTRLEN-n); };

void mac_pasteconsole(char *commandbuf)
{	
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    NSString *type = [pasteboard availableTypeFromArray:[NSArray arrayWithObject:NSStringPboardType]];
    if (type != nil) {
        NSString *contents = [pasteboard stringForType:type];
        if (contents != nil)
			s_strcat(commandbuf, [contents cStringUsingEncoding:NSASCIIStringEncoding]); // 10.4+
    }
}

/*
 * 0x0A0400 = 10.4
 * 0x0A0500 = 10.5
 * 0x0A0600 = 10.6
 */
int mac_osversion() 
{
    SInt32 majorVersion = 0, minorVersion = 0, bugVersion = 0;
    Gestalt(gestaltSystemVersionMajor, &majorVersion);
    Gestalt(gestaltSystemVersionMinor, &minorVersion);
    Gestalt(gestaltSystemVersionBugFix, &bugVersion);
    return (majorVersion<<16) | (minorVersion<<8) | bugVersion;
}
