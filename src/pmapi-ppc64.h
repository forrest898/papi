#ifndef _PAPI_PMAPI_PPC64              /* _PAPI_PMAPI_PPC64 */
#define _PAPI_PMAPI_PPC64

#include "aix.h"

#define PM_INIT_FLAGS PM_VERIFIED|PM_UNVERIFIED|PM_CAVEAT|PM_GET_GROUPS

#if defined(_POWER4) || defined(_POWER5)
typedef PMINFO_T hwd_pminfo_t;
typedef PMEVENTS_T hwd_pmevents_t;
#endif

#include "ppc64_events.h"

typedef struct ppc64_pmapi_control {
   /* Buffer to pass to the kernel to control the counters */
   pm_prog_t counter_cmd;
   int group_id;
   /* Space to read the counters */
   pm_data_t state;
} ppc64_pmapi_control_t;

typedef struct ppc64_reg_alloc {
   int ra_position;
   unsigned int ra_group[GROUP_INTS];
   int ra_counter_cmd[MAX_COUNTERS];
} ppc64_reg_alloc_t;

/*typedef struct PWR4_register {*/
  /* unsigned int event_code; */
  /* register number the corespondent native event in the event lives on */
  /* unsigned char pos[MAX_COUNTERS];  
     } PWR4_register_t; */

typedef struct ppc64_pmapi_context {
   /* this structure is a work in progress */
   ppc64_pmapi_control_t cntrl;
} ppc64_pmapi_context_t;

typedef ppc64_pmapi_control_t hwd_control_state_t;

/*typedef ppc64_register_t hwd_register_t;*/

typedef ppc64_reg_alloc_t hwd_reg_alloc_t;

typedef ppc64_pmapi_context_t hwd_context_t;

/* ... for PAPI3
typedef ppc64_pmapi_event_t hwd_event_t;
*/

/*
typedef struct hwd_groups {
  // group number from the pmapi pm_groups_t struct 
  //int group_id;
  // Buffer containing counter cmds for this group 
  unsigned char counter_cmd[POWER_MAX_COUNTERS];
} hwd_groups_t;
*/
/* prototypes */
extern int set_domain(hwd_control_state_t * this_state, int domain);
extern int set_granularity(hwd_control_state_t * this_state, int domain);
extern int _papi_hwd_init_preset_search_map(hwd_pminfo_t * info);

#endif                          /* _PAPI_PMAPI_PPC64 */