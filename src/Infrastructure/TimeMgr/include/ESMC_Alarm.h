// $Id: ESMC_Alarm.h,v 1.15 2003/08/07 16:52:10 eschwab Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2003, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the GPL.
//
// ESMF TimeInstant C++ definition include file
//
// (all lines below between the !BOP and !EOP markers will be included in
//  the automated document processing.)
//-------------------------------------------------------------------------
//
 // these lines prevent this file from being read more than once if it
 // ends up being included multiple times

#ifndef ESMC_ALARM_H
#define ESMC_ALARM_H

//-------------------------------------------------------------------------

 // put any constants or macros which apply to the whole component in this file.
 // anything public or esmf-wide should be up higher at the top level
 // include files.
 #include <ESMF_TimeMgr.inc>
 #include <pthread.h>

//-------------------------------------------------------------------------
//BOP
// 
// !CLASS: ESMC_Alarm - maintains ringing times and ringing state
// 
// !DESCRIPTION:
//
// The code in this file defines the C++ {\tt Alarm} members and method
// signatures (prototypes).  The companion file {\tt ESMC\_Alarm.C} contains
// the full code (bodies) for the {\tt Alarm} methods.
//
// The {\tt Alarm} class encapsulates the required alarm behavior, triggering
// its ringing state on either a one-shot or repeating interval basis.
//
// The {\tt Alarm} class contains {\tt Time} instants and a {\tt TimeInterval}
// to perform one-shot and interval alarming.  A single {\tt TimeInterval}
// holds the alarm interval if used.  A {\tt Time} instant is defined for the
// ring time, used for either the one-shot alarm time or for the next interval
// alarm time.  A {\tt Time} instant is also defined for the previous ring
// time to keep track of alarm intervals.  A {\tt Time} instant for stop time
// defines when alarm intervals end.  If a one-shot alarm is defined, only
// the ring time attribute is used, the others are not.  To keep track of
// alarm state, two logical attributes are defined, one for ringing on/off,
// and the other for alarm enabled/disabled.  An alarm is enabled by default;
// if disabled by the user, it does not function at all.
//
// The primary method is to check whether it is time to set the ringer, which
// is called by the associated clock after performing a time step.  The clock
// will pass a parameter telling the alarm check method whether the ringer is
// to be set upon crossing the ring time in the positive or negative direction.
// This is to handle both positive and negative clock timesteps.  After the
// ringer is set for interval alarms, the check method will recalculate the
// next ring time.  This can be in the positve or negative direction, again
// depending on the parameter passed in by the clock.
//
// Other methods are defined for getting the ringing state, turning the
// ringer on/off, enabling/disabling the alarm, and getting/setting the
// time attributes defined above.
//
// Notes:
//    TMG 4.1, 4.2:  Multiple alarms may be instantiated and associated
//                   with a clock via clock methods
//
//-------------------------------------------------------------------------
//
// !USES:
 #include <ESMC_Base.h>
 #include <ESMC_TimeInterval.h>
 #include <ESMC_Time.h>

// !PUBLIC TYPES:
 class ESMC_Alarm;

// !PRIVATE TYPES:
 // class configuration type:  not needed for Alarm

 // class definition type
class ESMC_Alarm {
//class ESMC_Alarm : public ESMC_Base { // TODO: inherit from ESMC_Base class
                                        // when fully aligned with F90 equiv

  private:   // corresponds to F90 module 'type ESMF_Alarm' members
    ESMC_TimeInterval RingInterval; // (TMG 4.5.2) for periodic alarming
    ESMC_TimeInterval ringDuration; // how long alarm stays on
    ESMC_Time         RingTime;     // (TMG 4.5.1) next time to ring
    ESMC_Time         PrevRingTime; // previous alarm time 
    ESMC_Time         StopTime;     // when alarm intervals end.
    ESMC_Time         ringBegin;    // note time when alarm turns on.
    ESMC_Time         refTime;      // reference time.

    int               ID;         // used to distinguish among
                                  //   multiple clock alarms
//    pthread_mutex_t   AlarmMutex; // (TMG 7.5)

    bool              Ringing;    // (TMG 4.4) currently ringing
    bool              Enabled;    // able to ring (TMG 4.5.3)
    bool              sticky;     // must be turned off via ESMC_AlarmTurnOff(),
                                  //  otherwise will turn self off after
                                  //  ringDuration.
    bool              pad1;       //  align on byte boundary

// !PUBLIC MEMBER FUNCTIONS:

  public:

    // Alarm doesn't need configuration, hence GetConfig/SetConfig
    // methods are not required

    // accessor methods

    int ESMC_AlarmSet(ESMC_Time         *ringTime=0,
                      ESMC_TimeInterval *ringInterval=0,
                      ESMC_Time         *stopTime=0,
                      bool               enabled=true);  // (TMG 4.1, 4.7)

    int ESMC_AlarmGetRingInterval(ESMC_TimeInterval *ringInterval) const;
                                                           // TMG4.7
    int ESMC_AlarmSetRingInterval(ESMC_TimeInterval *ringInterval);
                                                           // TMG4.5.2, 4.7

    int ESMC_AlarmGetRingDuration(ESMC_TimeInterval *ringduration) const;
    int ESMC_AlarmSetRingDuration(ESMC_TimeInterval *ringduration,
                                  int numTimeSteps, 
                                  ESMC_TimeInterval *timestep);

    int ESMC_AlarmGetRingTime(ESMC_Time *ringTime) const;
                                                           // TMG4.7, 4.8
    int ESMC_AlarmSetRingTime(ESMC_Time *ringTime); 
                                                           // TMG4.5.1, 4.7, 4.8

    int ESMC_AlarmGetPrevRingTime(ESMC_Time *prevRingTime) const;
                                                           // TMG 4.7, 4.8
    int ESMC_AlarmSetPrevRingTime(ESMC_Time *prevRingTime);
                                                           // TMG 4.7, 4.8

    int ESMC_AlarmGetStopTime(ESMC_Time *stopTime) const;  // TMG 4.5.2, 4.7
    int ESMC_AlarmSetStopTime(ESMC_Time *stopTime);        // TMG 4.5.2, 4.7

    int ESMC_AlarmGetRefTime(ESMC_Time *reftime) const;
    int ESMC_AlarmSetRefTime(ESMC_Time *reftime);

    int ESMC_AlarmEnable(void);    // TMG4.5.3
    int ESMC_AlarmDisable(void);

    int ESMC_AlarmTurnOn(void);    // TMG4.6: manually turn on/off
    int ESMC_AlarmTurnOff(void);

    int ESMC_AlarmSticky(void);
    int ESMC_AlarmNotSticky(void);

    bool ESMC_AlarmIsRinging(int *rc) const;
                                         // TMG 4.4: synchronous query for apps
    bool ESMC_AlarmCheckRingTime(ESMC_Time *clockCurrTime, bool positive,
                                 int *rc);
                         // associated clock should invoke after advance:
                         // TMG4.4, 4.6
                         // Check for crossing RingTime in either positive or
                         //   negative direction
                         // Can be basis for asynchronous alarm reporting

    bool operator==(const ESMC_Alarm &) const; 

    // required methods inherited and overridden from the ESMC_Base class

    // for persistence/checkpointing

    // restore state
    int ESMC_AlarmReadRestart(ESMC_TimeInterval *ringInterval,
                              ESMC_TimeInterval *ringduration,
                              ESMC_Time         *ringTime,
                              ESMC_Time         *prevRingTime,
                              ESMC_Time         *stopTime,
                              ESMC_Time         *ringbegin,
                              ESMC_Time         *reftime,
                              bool              ringing,
                              bool              enabled,
                              bool              Sticky,
                              int               id);

    // save state
    int ESMC_AlarmWriteRestart(ESMC_TimeInterval *ringInterval,
                               ESMC_TimeInterval *ringduration,
                               ESMC_Time         *ringTime,
                               ESMC_Time         *prevRingTime,
                               ESMC_Time         *stopTime,
                               ESMC_Time         *ringbegin,
                               ESMC_Time         *reftime,
                               bool              *ringing,
                               bool              *enabled,
                               bool              *Sticky,
                               int               *id) const;

    // internal validation
    int ESMC_AlarmValidate(const char *options=0) const;

    // for testing/debugging
    int ESMC_AlarmPrint(const char *options=0) const;

    // native C++ constructors/destructors
    ESMC_Alarm(void);
    ~ESMC_Alarm(void);

 // < declare the rest of the public interface methods here >

// !PRIVATE MEMBER FUNCTIONS:
//
  private:
//
 // < declare private interface methods here >

//
//EOP
//-------------------------------------------------------------------------

};  // end class ESMC_Alarm

#endif // ESMC_ALARM_H
