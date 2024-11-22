#include "wa.fft.peaks_tidle.h"

namespace c74
{
   namespace max
   {
#include <r_pfft.h> // public pfft struct in r_pfft.h
   } // namespace max
} // namespace c74

#include <MaxPatcher.h>

FourierPeaks::FourierPeaks(const atoms& args)
   : object<FourierPeaks>()
   , dspsetup(this, "dspsetup", Max::Patcher::minBind(this, &FourierPeaks::dspSetupFunction))
   , peaks()
{
   // peaks demo
   {
      std::srand(std::time(nullptr));

      peaks.resize(100);

      for (int index = 0; index < 100; index++)
      {
         const double value = std::rand() / static_cast<double>(RAND_MAX);
         peaks[index] = Peak{index, value};
      }

      std::sort(peaks.begin(), peaks.end(), std::greater<Peak>());
   }
}

atoms FourierPeaks::dspSetupFunction(const atoms& args, const int inlet)
{
   // fft stuff
   c74::max::t_symbol* symbol_pfft = c74::max::gensym("__pfft~__");
   if (!symbol_pfft)
   {
      cout << "wa.fft.peaks~ only functions inside a pfft~" << endl;
      return {};
   }

   c74::max::t_pfftpub* pfft = (c74::max::t_pfftpub*)symbol_pfft->s_thing;

   int fullspect = pfft->x_fullspect ? 1 : 0;
   long ffthop = pfft->x_ffthop;
   long fftsize = pfft->x_fftsize;

   cout << "fullspect: " << fullspect << endl;
   cout << "ffthop: " << ffthop << endl;
   cout << "fftsize: " << fftsize << endl;

   //long vectorSize = sp[0]->s_n;
   return {};
}

MIN_EXTERNAL(FourierPeaks);