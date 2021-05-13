--- common/autoboot.c.orig	2021-04-20 16:09:41.052888000 +0300
+++ common/autoboot.c	2021-04-20 16:10:03.018634000 +0300
@@ -258,6 +258,10 @@
 	 */
 	if (tstc()) {	/* we got a key press	*/
 		(void) getc();  /* consume input	*/
+
+ 		env_set("stderr", "serial,vidconsole");	
+ 		env_set("stdout", "serial,vidconsole"); /* enable vidconsole if key press */
+
 		puts("\b\b\b 0");
 		abort = 1;	/* don't auto boot	*/
 	}
@@ -270,9 +274,12 @@
 			if (tstc()) {	/* we got a key press	*/
 				int key;
 
+ 				env_set("stderr", "serial,vidconsole");	
+ 				env_set("stdout", "serial,vidconsole"); /* enable vidconsole if key press */
+
 				abort  = 1;	/* don't auto boot	*/
 				bootdelay = 0;	/* no more delay	*/
-				key = getc(); /* consume input	*/
+				key = getc();	/* consume input	*/
 				if (IS_ENABLED(CONFIG_USE_AUTOBOOT_MENUKEY))
 					menukey = key;
 				break;
