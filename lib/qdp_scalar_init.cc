// $Id: qdp_scalar_init.cc,v 1.6 2004-07-27 05:36:36 edwards Exp $

/*! @file
 * @brief Scalar init routines
 * 
 * Initialization routines for scalar implementation
 */


#include "qdp.h"

QDP_BEGIN_NAMESPACE(QDP);


//! Private flag for status
static bool isInit = false;

//! Turn on the machine
void QDP_initialize(int *argc, char ***argv) 
{
  if (isInit)
    QDP_error_exit("QDP already inited");

  Layout::init();   // setup extremely basic functionality in Layout

  isInit = true;

  // initialize remote file service (QIO)
  QDPUtil::RemoteFileInit("qcdi01", false);

  // initialize the global streams
  QDPIO::cin.init(&std::cin);
  QDPIO::cout.init(&std::cout);
  QDPIO::cerr.init(&std::cerr);

  //
  // Process command line
  //

  // Look for help
  bool help_flag = false;
  for (int i=0; i<*argc; i++) 
  {
    if (strcmp((*argv)[i], "-h")==0)
      help_flag = true;
  }

  if (help_flag) 
  {
    fprintf(stderr,"Usage:    %s options\n",(*argv)[0]);
    fprintf(stderr,"options:\n");
    fprintf(stderr,"    -h        help\n");
#if defined(QDP_USE_PROFILING)   
    fprintf(stderr,"    -p        %%d [%d] profile level\n", 
	    getProfileLevel());
#endif

    exit(1);
  }

  for (int i=1; i<*argc; i++) 
  {
#if defined(QDP_USE_PROFILING)   
    if (strcmp((*argv)[i], "-p")==0) 
    {
      int lev;
      sscanf((*argv)[++i], "%d", &lev);
      setProgramProfileLevel(lev);
    }
#endif

    if (i >= *argc) 
    {
      QDP_error_exit("missing argument at the end");
    }
  }

  initProfile(__FILE__, __func__, __LINE__);
}

//! Is the machine initialized?
bool QDP_isInitialized() {return isInit;}

//! Turn off the machine
void QDP_finalize()
{
  printProfile();

  // shutdown remote file service (QIO)
  QDPUtil::RemoteFileShutdown();

  isInit = false;
}

//! Panic button
void QDP_abort(int status)
{
  QDP_finalize(); 
  exit(status);
}

//! Resumes QDP communications
void QDP_resume() {}

//! Suspends QDP communications
void QDP_suspend() {}


QDP_END_NAMESPACE();
