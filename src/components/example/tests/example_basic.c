/****************************/
/* THIS IS OPEN SOURCE CODE */
/****************************/

/** 
 * @file    example_basic.c
 * @author  Vince Weaver
 *	    vweaver1@eecs.utk.edu
 * test case for Example component 
 * 
 *
 * @brief
 *  This file is a very simple example test and Makefile that acat
 *	as a guideline on how to add tests to components.
 *  The papi configure and papi Makefile will take care of the compilation
 *	of the component tests (if all tests are added to a directory named
 *	'tests' in the specific component dir).
 *	See components/README for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include "papi_test.h"

#define NUM_EVENTS 3

int main (int argc, char **argv)
{

        int retval,i;
	int EventSet = PAPI_NULL;
	long long values[NUM_EVENTS];
	const PAPI_component_info_t *cmpinfo = NULL;
	int numcmp,cid,example_cid=-1;
	int code,maximum_code=0;
	char event_name[PAPI_MAX_STR_LEN];
	PAPI_event_info_t event_info;

        /* Set TESTS_QUIET variable */
        tests_quiet( argc, argv );      

	/* PAPI Initialization */
	retval = PAPI_library_init( PAPI_VER_CURRENT );
	if ( retval != PAPI_VER_CURRENT ) {
	   test_fail(__FILE__, __LINE__,"PAPI_library_init failed\n",retval);
	}

	if (!TESTS_QUIET) {
	   printf( "Testing example component with PAPI %d.%d.%d\n",
			PAPI_VERSION_MAJOR( PAPI_VERSION ),
			PAPI_VERSION_MINOR( PAPI_VERSION ),
			PAPI_VERSION_REVISION( PAPI_VERSION ) );
	}

	/* Find our component */

	numcmp = PAPI_num_components();
	for( cid=0; cid<numcmp; cid++) {
	   if ( (cmpinfo = PAPI_get_component_info(cid)) == NULL) {
	      test_fail(__FILE__, __LINE__,
                           "PAPI_get_component_info failed\n", 0);
	   }
	   if (!TESTS_QUIET) {
	      printf("\tComponent %d - %s\n", cid, cmpinfo->name);
	   }
	   if (strstr(cmpinfo->name,"example.c")) {
	      /* FOUND! */
	      example_cid=cid;
	   }
	}
	

	if (example_cid<0) {
	   test_skip(__FILE__, __LINE__,
		     "Example component not found\n", 0);
	}

	if (!TESTS_QUIET) {
	  printf("\nFound Example Component at id %d\n",example_cid);
	  printf("\nListing all events in this component:\n");
	}

	/**************************************************/
	/* Listing all available events in this component */
	/* Along with descriptions                        */
	/**************************************************/
	code = PAPI_NATIVE_MASK | PAPI_COMPONENT_MASK(example_cid);

	retval = PAPI_enum_event( &code, PAPI_ENUM_FIRST );

	while ( retval == PAPI_OK ) {
	  if (PAPI_event_code_to_name( code, event_name )!=PAPI_OK) {
	     printf("Error translating %x\n",code);
	     test_fail( __FILE__, __LINE__, 
		       "PAPI_event_code_to_name", retval );
	  }

	  if (PAPI_get_event_info( code, &event_info)!=PAPI_OK) {
	     printf("Error getting info for event %x\n",code);
	     test_fail( __FILE__, __LINE__, 
		       "PAPI_get_event_info()", retval );
	  }

	  if (!TESTS_QUIET) {
	    printf("\tEvent 0x%x: %s -- %s\n",
		   code,event_name,event_info.long_descr);
	  }

	  maximum_code=code;

	  retval = PAPI_enum_event( &code, PAPI_ENUM_EVENTS );

	}
	if (!TESTS_QUIET) printf("\n");

	/**********************************/
	/* Try accessing an invalid event */
	/**********************************/
	
	retval=PAPI_event_code_to_name( maximum_code+10, event_name );
	if (retval!=PAPI_ENOEVNT) {
	   test_fail( __FILE__, __LINE__, 
		    "Failed to return PAPI_ENOEVNT on invalid event", retval );
	}

	/***********************************/
	/* Test the EXAMPLE_ZERO event     */
	/***********************************/

	retval = PAPI_create_eventset( &EventSet );
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__,
		      "PAPI_create_eventset() failed\n", retval );
	}

	retval = PAPI_event_name_to_code("EXAMPLE_ZERO", &code);
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__, 
		      "EXAMPLE_ZERO not found\n",retval );
	}

	retval = PAPI_add_event( EventSet, code);
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__,
		      "PAPI_add_events failed\n", retval );
	}

	retval = PAPI_start( EventSet );
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__, 
		      "PAPI_start failed\n",retval );
	}
	
	retval = PAPI_stop( EventSet, values );
	if ( retval != PAPI_OK ) {
	   test_fail(  __FILE__, __LINE__, "PAPI_stop failed\n", retval);
	}

	if (!TESTS_QUIET) printf("Testing EXAMPLE_ZERO: %lld\n",values[0]);

	if (values[0]!=0) {
	   test_fail(  __FILE__, __LINE__, "Result should be 0!\n", 0);
	}

	retval = PAPI_cleanup_eventset(EventSet);
	if (retval != PAPI_OK) {
	   test_fail(  __FILE__, __LINE__, "PAPI_cleanup_eventset!\n", retval);
	}

	retval = PAPI_destroy_eventset(&EventSet);
	if (retval != PAPI_OK) {
	   test_fail(  __FILE__, __LINE__, "PAPI_destroy_eventset!\n", retval);
	}

	EventSet=PAPI_NULL;


	/***********************************/
	/* Test the EXAMPLE_CONSTANT event */
	/***********************************/

	retval = PAPI_create_eventset( &EventSet );
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__,
		      "PAPI_create_eventset() failed\n", retval );
	}

	retval = PAPI_event_name_to_code("EXAMPLE_CONSTANT", &code);
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__, 
		      "EXAMPLE_CONSTANT not found\n",retval );
	}

	retval = PAPI_add_event( EventSet, code);
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__,
		      "PAPI_add_events failed\n", retval );
	}

	retval = PAPI_start( EventSet );
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__, 
		      "PAPI_start failed\n",retval );
	}
	
	retval = PAPI_stop( EventSet, values );
	if ( retval != PAPI_OK ) {
	   test_fail(  __FILE__, __LINE__, "PAPI_stop failed\n", retval);
	}

	if (!TESTS_QUIET) printf("Testing EXAMPLE_CONSTANT: %lld\n",values[0]);

	if (values[0]!=42) {
	   test_fail(  __FILE__, __LINE__, "Result should be 42!\n", 0);
	}

	retval = PAPI_cleanup_eventset(EventSet);
	if (retval != PAPI_OK) {
	   test_fail(  __FILE__, __LINE__, "PAPI_cleanup_eventset!\n", retval);
	}

	retval = PAPI_destroy_eventset(&EventSet);
	if (retval != PAPI_OK) {
	   test_fail(  __FILE__, __LINE__, "PAPI_destroy_eventset!\n", retval);
	}

	EventSet=PAPI_NULL;



	/***********************************/
	/* Test the EXAMPLE_AUTOINC event  */
	/***********************************/

	retval = PAPI_create_eventset( &EventSet );
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__,
		      "PAPI_create_eventset() failed\n", retval );
	}

	retval = PAPI_event_name_to_code("EXAMPLE_AUTOINC", &code);
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__, 
		      "EXAMPLE_AUTOINC not found\n",retval );
	}

	retval = PAPI_add_event( EventSet, code);
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__,
		      "PAPI_add_events failed\n", retval );
	}

	if (!TESTS_QUIET) printf("Testing EXAMPLE_AUTOINC: ");

	for(i=0;i<10;i++) {

	   retval = PAPI_start( EventSet );
	   if ( retval != PAPI_OK ) {
	      test_fail( __FILE__, __LINE__, 
		      "PAPI_start failed\n",retval );
	   }
	
	   retval = PAPI_stop( EventSet, values );
	   if ( retval != PAPI_OK ) {
	      test_fail(  __FILE__, __LINE__, "PAPI_stop failed\n", retval);
	   }

	   if (!TESTS_QUIET) printf("%lld ",values[0]);

	   if (values[0]!=i) {
	      test_fail(  __FILE__, __LINE__, "Result wrong!\n", 0);
	   }
	}

	if (!TESTS_QUIET) printf("\n");


	/***********************************/
	/* Test PAPI_reset()               */
	/***********************************/

	retval = PAPI_reset( EventSet );
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__, 
		      "PAPI_reset() failed\n",retval );
	}

	retval = PAPI_start( EventSet );
	if ( retval != PAPI_OK ) {
	   test_fail( __FILE__, __LINE__, 
		      "PAPI_start failed\n",retval );
	}
	
	retval = PAPI_stop( EventSet, values );
	if ( retval != PAPI_OK ) {
	   test_fail(  __FILE__, __LINE__, "PAPI_stop failed\n", retval);
	}

	if (!TESTS_QUIET) printf("Testing EXAMPLE_AUTOINC after PAPI_reset(): %lld\n",
				 values[0]);

	if (values[0]!=0) {
	  //	   test_fail(  __FILE__, __LINE__, "Result not zero!\n", 0);
	}

	retval = PAPI_cleanup_eventset(EventSet);
	if (retval != PAPI_OK) {
	   test_fail(  __FILE__, __LINE__, "PAPI_cleanup_eventset!\n", retval);
	}

	retval = PAPI_destroy_eventset(&EventSet);
	if (retval != PAPI_OK) {
	   test_fail(  __FILE__, __LINE__, "PAPI_destroy_eventset!\n", retval);
	}

	EventSet=PAPI_NULL;


	/***********************************/
	/* Test multiple events            */
	/***********************************/

	/***********************************/
	/* Test writing to an event        */
	/***********************************/

	/* TODO */

	if (!TESTS_QUIET) printf("\n");

	test_pass( __FILE__, NULL, 0 );
		
	return 0;
}
