/*
 *  main.c
 *
 *  $Id$
 *
 *  Main program
 *
 *  The iODBC driver manager.
 *
 *  Copyright (C) 2001 by OpenLink Software <iodbc@openlinksw.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iodbc.h>
#include <isql.h>
#include <iodbcinst.h>
#include <unistd.h>
#include <stdlib.h>

#include "gui.h"

int
gtk_gui (int *argc, char **argv[])
{
  GtkWidget *mainwnd;
  gtk_init (argc, argv);
  mainwnd = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  return SQLManageDataSources (mainwnd);
}


int
kde_gui (int *argc, char **argv[])
{
  return -1;
}


void
display_help (void)
{
  printf ("-help\t\t\tDisplay the list of options.\n\r");
  printf
      ("-odbc filename\t\tSet the location of the user ODBC.INI file.\n\r");
  printf
      ("-odbcinst filename\tSet the location of the user ODBCINST.INI file.\n\r");
  printf
      ("-sysodbc filename\tSet the location of the system ODBC.INI file.\n\r");
  printf
      ("-sysodbcinst filename\tSet the location of the system ODBCINST.INI file.\n\r");
  printf ("-gui guitype\t\tSet the GUI type : GTK, KDE.\n\r");
  printf ("-debug\t\t\tThe error messages are displayed on the console.\n\r");
  printf
      ("-admin odbcinstfile\tUsed to administrate the system odbcinst.ini file.\n\r\n\r");
  _exit (1);
}


int
main (int argc, char *argv[])
{
  BOOL debug = FALSE;
  char path[4096];
  char *gui = NULL;
  int i = 1;

  printf ("iODBC Administrator from OpenLink Software, Copyright 2000.\n\r");
  printf ("Build " DATEBUILD ", compliant ODBC v" VERSION "\n\r\n\r");

  /* Check options commands */
  if (argc > 1)
    {
      for (; i < argc; i++)
	{
	  if (!strcasecmp (argv[i], "-help"))
	    display_help ();

	  if (!strcasecmp (argv[i], "-debug"))
	    debug = TRUE;

	  if (!strcasecmp (argv[i], "-odbc"))
	    {
	      if (i + 1 >= argc)
		display_help ();
	      setenv ("ODBCINI", argv[++i], TRUE);
	    }

	  if (!strcasecmp (argv[i], "-admin"))
	    {
	      if (i + 1 >= argc)
		display_help ();
	      setenv ("ODBCINSTINI", argv[++i], TRUE);
	      setenv ("SYSODBCINSTINI", argv[i], TRUE);
	    }

	  if (!strcasecmp (argv[i], "-odbcinst"))
	    {
	      if (i + 1 >= argc)
		display_help ();
	      setenv ("ODBCINSTINI", argv[++i], TRUE);
	    }

	  if (!strcasecmp (argv[i], "-sysodbc"))
	    {
	      if (i + 1 >= argc)
		display_help ();
	      setenv ("SYSODBCINI", argv[++i], TRUE);
	    }

	  if (!strcasecmp (argv[i], "-sysodbcinst"))
	    {
	      if (i + 1 >= argc)
		display_help ();
	      setenv ("SYSODBCINSTINI", argv[++i], TRUE);
	    }

	  if (!strcasecmp (argv[i], "-gui"))
	    {
	      if (i + 2 >= argc)
		display_help ();
	      gui = argv[++i];
	    }
	}
    }

  if (!getenv ("ODBCINI") && getenv ("HOME"))
    {
      STRCPY (path, getenv ("HOME"));
      STRCAT (path, "/.odbc.ini");
      setenv ("ODBCINI", path, TRUE);
    }

  if (!debug)
    {
      close (STDOUT_FILENO);
      close (STDERR_FILENO);
    }

  if (gui && !strcasecmp (gui, "KDE"))
    return kde_gui (&argc, &argv);

  return gtk_gui (&argc, &argv);
}
