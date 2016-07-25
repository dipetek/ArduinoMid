//
// Created by Admin on 7/24/2016.
//

#ifndef ARDUINOMID_CONSUMPTION_H
#define ARDUINOMID_CONSUMPTION_H

#include "lib/sensors/Tachometer.h"

static int getVolumetricEfficiency (rpm_var);

static int calcConsumption (void)
{
  int rpmVar = getDigitalTachometerRpm ();
  int VolumetricEfficiency = getVolumetricEfficiency (rpmVar);

  IMAP = double (rpmVar * dvk_var) / double (intake_air_temp_var + 273.15);
  MAF = double (IMAP / 120.0) * double (double (VolumetricEfficiency * VEC_FUL_RT) / 100.0) * CON_ENG_CC * 28.9644
        / 8.314472;

  if (fss_val == 2)
    {   // если замкнутая обратная связь  - Closed Loop
      ls_term_val = double (100.0 + (long_term_val + short_term_val))
                    / 100.0; // коэффициент корректировки расхода по ShortTerm и LongTerm
    }
  else
    {
      ls_term_val = double (100.0 + long_term_val) / 100.0; // коэффициент корректировки расхода по LongTerm
    }

}

static int getVolumetricEfficiency (rpm_var)
{
  int VE;   // VE -  volumetric efficiency measured in percent, let's say 80%
  if (rpm_var < 800)
    {
      VE = 64;
    }
  else
    {
      if (rpm_var < 1001)
        {
          VE = 66;
        }
      else
        {
          if (rpm_var < 1251)
            {
              VE = 70;
            }
          else
            {

              if (rpm_var < 1501)
                {
                  VE = 70;
                }
              else
                {

                  if (rpm_var < 1751)
                    {
                      VE = 74;
                    }
                  else
                    {

                      if (rpm_var < 2001)
                        {
                          VE = 74;
                        }
                      else
                        {

                          if (rpm_var < 2126)
                            {
                              VE = 76;
                            }
                          else
                            {

                              if (rpm_var < 2251)
                                {
                                  VE = 77;
                                }
                              else
                                {

                                  if (rpm_var < 2376)
                                    {
                                      VE = 79;
                                    }
                                  else
                                    {

                                      if (rpm_var < 2501)
                                        {
                                          VE = 80;
                                        }
                                      else
                                        {

                                          if (rpm_var < 2626)
                                            {
                                              VE = 82;
                                            }
                                          else
                                            {

                                              if (rpm_var < 2751)
                                                {
                                                  VE = 82;
                                                }
                                              else
                                                {

                                                  if (rpm_var < 2876)
                                                    {
                                                      VE = 83;
                                                    }
                                                  else
                                                    {

                                                      if (rpm_var < 3001)
                                                        {
                                                          VE = 84;
                                                        }
                                                      else
                                                        {

                                                          if (rpm_var < 3501)
                                                            {
                                                              VE = 85;
                                                            }
                                                          else
                                                            {

                                                              if (rpm_var < 4001)
                                                                {
                                                                  VE = 86;
                                                                }
                                                              else
                                                                {

                                                                  if (rpm_var < 4501)
                                                                    {
                                                                      VE = 88;
                                                                    }
                                                                  else
                                                                    {

                                                                      if (rpm_var < 5001)
                                                                        {
                                                                          VE = 90;
                                                                        }
                                                                      else
                                                                        {

                                                                          if (rpm_var < 5501)
                                                                            {
                                                                              VE = 93;
                                                                            }
                                                                          else
                                                                            {

                                                                              if (rpm_var < 6001)
                                                                                {
                                                                                  VE = 95;
                                                                                }
                                                                              else
                                                                                {

                                                                                  if (rpm_var < 6501)
                                                                                    {
                                                                                      VE = 91;
                                                                                    }
                                                                                  else
                                                                                    {

                                                                                      if (rpm_var < 7000)
                                                                                        {
                                                                                          VE = 85;
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
  return VE;
}

#endif //ARDUINOMID_CONSUMPTION_H
