/* --------------------------------------------------------------------------

   MusicBrainz -- The Intenet music metadatabase

   Portions Copyright (C) 2000 Relatable
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

   $Id$

----------------------------------------------------------------------------*/

#include "musicbrainz.h"
#include "sigfft.h"
#include "sigclient.h"

#include <stdio.h>
#include <stdlib.h>

void MusicBrainz::SetPCMDataInfo(int samplesPerSecond, int numChannels,
                                 int bitsPerSample)
{
    m_samples_per_second = samplesPerSecond;
    m_number_of_channels = numChannels;
    m_bits_per_sample = bitsPerSample;

    if (m_downmixBuffer) {
        delete [] m_downmixBuffer;
        m_downmixBuffer = NULL;
    }
    if (m_storeBuffer) {
        delete [] m_storeBuffer;
        m_storeBuffer = NULL;
    }
    m_numSamplesWritten = 0;

    float mult = (float)m_samples_per_second / 11025.0;
    mult *= (m_bits_per_sample / 8);
    mult *= (m_number_of_channels);
    mult = ceil(mult);

cout << m_samples_per_second << " " << m_bits_per_sample << " " << m_number_of_channels << endl;
    m_numRealSamplesWritten = 0;
    m_numRealSamplesNeeded = iNumSamplesNeeded * (int)mult;
    m_storeBuffer = new char[m_numRealSamplesNeeded + 20];
}

bool MusicBrainz::GenerateSignature(char *data, int size, string &strGUID,
                                    string &collID)
{
   if (m_numRealSamplesWritten < m_numRealSamplesNeeded) {
       int i = 0;
       while (i < size && m_numRealSamplesWritten < m_numRealSamplesNeeded) {
           m_storeBuffer[m_numRealSamplesWritten] = data[i];
           m_numRealSamplesWritten++;
           i++;
       }
   }

   if (m_numRealSamplesWritten < m_numRealSamplesNeeded)
       return false;

   GenerateSignatureNow(strGUID, collID);

   return true;

}

void MusicBrainz::DownmixPCM(void)
{
   // DC Offset fix
   long int lsum = 0, rsum = 0;
   long int numsamps = 0;
   int lDC = 0, rDC = 0;
   signed short lsample, rsample;
   unsigned char ls, rs;
   int readpos = 0;
    
char filename[1024];
sprintf(filename, "/tmp/1-%d.out", time(NULL));
FILE *outfile = fopen(filename, "w");
fwrite(m_storeBuffer, sizeof(signed short), m_numRealSamplesWritten / 2, outfile);
fclose(outfile);

   if (m_bits_per_sample == 16) {
       if (m_number_of_channels == 2) {
           while (readpos < (m_numRealSamplesWritten / 2)) {
               lsample = ((signed short *)m_storeBuffer)[readpos++];
               rsample = ((signed short *)m_storeBuffer)[readpos++];
               
               lsum += lsample; 
               rsum += rsample;
               numsamps++;
           }
           lDC = -(lsum / numsamps);
           rDC = -(rsum / numsamps);

           cout << "left offset " << lDC << endl;
           cout << "right offset " << rDC << endl;

           readpos = 0;
           while (readpos < (m_numRealSamplesWritten / 2)) {
               ((signed short *)m_storeBuffer)[readpos] = 
                    ((signed short *)m_storeBuffer)[readpos] + lDC;
               readpos++;
               ((signed short *)m_storeBuffer)[readpos] =
                    ((signed short *)m_storeBuffer)[readpos] + rDC;
               readpos++;
           }
       }
       else {
           while (readpos < m_numRealSamplesWritten / 2) {
               lsample = ((signed short *)m_storeBuffer)[readpos++];
               
               lsum += lsample;
               numsamps++;
           }

           lDC = -(lsum / numsamps);
 
           cout << "offset " << lDC << endl;

           readpos = 0;
           while (readpos < m_numRealSamplesWritten / 2) {
               ((signed short *)m_storeBuffer)[readpos] =
                    ((signed short *)m_storeBuffer)[readpos] + lDC;
               readpos++;
           }
       }
    }
    else {
       if (m_number_of_channels == 2) {
           while (readpos < (m_numRealSamplesWritten)) {
               lsample = ((char *)m_storeBuffer)[readpos++];
               rsample = ((char *)m_storeBuffer)[readpos++];

               lsum += lsample;
               rsum += rsample;
               numsamps++;
           }
           lDC = -(lsum / numsamps);
           rDC = -(rsum / numsamps);

           cout << "left offset " << lDC << endl;
           cout << "right offset " << rDC << endl;

           readpos = 0;
           while (readpos < (m_numRealSamplesWritten)) {
               ((char *)m_storeBuffer)[readpos] =
                    ((char *)m_storeBuffer)[readpos] + lDC;
               readpos++;
               ((char *)m_storeBuffer)[readpos] =
                    ((char *)m_storeBuffer)[readpos] + rDC;
               readpos++;
           }
       }
       else {
           while (readpos < m_numRealSamplesWritten / 2) {
               lsample = ((char *)m_storeBuffer)[readpos++];

               lsum += lsample;
               numsamps++;
           }

           lDC = -(lsum / numsamps);

           cout << "offset " << lDC << endl;

           readpos = 0;
           while (readpos < m_numRealSamplesWritten / 2) {
               ((char *)m_storeBuffer)[readpos] =
                    ((char *)m_storeBuffer)[readpos] + lDC;
               readpos++;
           }
       }
    }

sprintf(filename, "/tmp/2-%d.out", time(NULL));
outfile = fopen(filename, "w");
fwrite(m_storeBuffer, sizeof(signed short), m_numRealSamplesWritten / 2, outfile);
fclose(outfile);

   if (!m_downmixBuffer)
       m_downmixBuffer = new unsigned char[iNumSamplesNeeded];

   m_downmix_size = m_numRealSamplesWritten;

   if (m_samples_per_second != 11025)
       m_downmix_size = (int)((float)m_downmix_size * 
                            (11025.0 / (float)m_samples_per_second));

   if (m_bits_per_sample != 8)
       m_downmix_size /= 2;

   if (m_number_of_channels != 1)
       m_downmix_size /= 2;

   int maxwrite = m_downmix_size;
   int writepos = 0;
   int rate_change = m_samples_per_second / 11025;

   if (m_bits_per_sample == 16) {
       if (m_number_of_channels == 2) {
           while ((writepos < maxwrite) && 
                  (m_numSamplesWritten < iNumSamplesNeeded)) 
           {
               readpos = writepos * rate_change;
               readpos *= 2;

               lsample = ((signed short *)m_storeBuffer)[readpos++];
               rsample = ((signed short *)m_storeBuffer)[readpos++];

               lsample /= 256; lsample += 127;
               rsample /= 256; rsample += 127;

               m_downmixBuffer[m_numSamplesWritten] = (lsample + rsample) / 2;
               m_numSamplesWritten++;
               writepos++;
           }
       }
       else {
           while ((writepos < maxwrite) &&
                  (m_numSamplesWritten < iNumSamplesNeeded))
           {
               readpos = writepos * rate_change;

               lsample = ((signed short *)m_storeBuffer)[readpos++];
               lsample /= 256; lsample += 127;

               m_downmixBuffer[m_numSamplesWritten] = lsample;
               m_numSamplesWritten++;
               writepos++;
           }
       }
   }
   else {
       if (m_number_of_channels == 2) {
           while ((writepos < maxwrite) &&  
                  (m_numSamplesWritten < iNumSamplesNeeded))
           {
               readpos = writepos * rate_change;
               readpos /= 2;

               ls = ((unsigned char *)m_storeBuffer)[readpos++];
               rs = ((unsigned char *)m_storeBuffer)[readpos++];

               m_downmixBuffer[m_numSamplesWritten] = (ls + rs) / 2;
               m_numSamplesWritten++;
               writepos++;
           }
       }
       else {
           while ((writepos < maxwrite) &&
                  (m_numSamplesWritten < iNumSamplesNeeded))
           {
               readpos = writepos * rate_change;
               
               ls = ((unsigned char *)m_storeBuffer)[readpos++];

               m_downmixBuffer[m_numSamplesWritten] = ls;
               m_numSamplesWritten++;
               writepos++;
           }
       }
   }

   delete [] m_storeBuffer;
   m_storeBuffer = NULL;
}

void MusicBrainz::GenerateSignatureNow(string &strGUID, string &collID)
{
    DownmixPCM();

char filename[1024];
sprintf(filename, "/tmp/3-%d.out", time(NULL));
FILE *outfile = fopen(filename, "w");
fwrite(m_downmixBuffer, sizeof(unsigned char), m_numSamplesWritten, outfile);
fclose(outfile);

    char *sample = (char *)m_downmixBuffer;  
    bool bLastNeg = false;
    if (*sample <= 0)
          bLastNeg = true;

    FFT *pFFT = new FFT(iFFTPoints, 11025);
    pFFT->CopyIn((char *)m_downmixBuffer, iFFTPoints);
    pFFT->Transform();

    char *pCurrent = (char *)m_downmixBuffer;
    char *pBegin = pCurrent;
    int iFFTs = m_numSamplesWritten / iFFTPoints;
    int j, k;

    int iSpectrum[iFFTPoints];
    for (j = 0; j < iFFTPoints; j++)
        iSpectrum[j] = 0;
    int iZeroCrossings = 0;
    double dEnergySum = 0.0;
    int iFinishedFFTs = 0;

    for (j = 0; j < iFFTs; j++, iFinishedFFTs++) {
        pFFT->CopyIn(pCurrent, iFFTPoints);
        pFFT->Transform();

        for (k = 0; k < iFFTPoints; k++)
            iSpectrum[k] += (int)pFFT->GetIntensity(k);

        while (pCurrent < pBegin + iFFTPoints)
        {
            dEnergySum += ((*pCurrent) * (*pCurrent));
            if (bLastNeg && (*pCurrent > 0))
            {
                bLastNeg = false;
                iZeroCrossings++;
            }
            else if (!bLastNeg && (*pCurrent <= 0))
                bLastNeg = true;
            pCurrent++;
        }
        pBegin = pCurrent;
    }

    float fLength = m_numSamplesWritten / (float)11025;
    float fAverageZeroCrossing = iZeroCrossings / fLength;
    float fEnergy = dEnergySum / (float)m_numSamplesWritten;
    for (int i = 0; i < iFFTPoints; i++)
        iSpectrum[i] = iSpectrum[i] / iFinishedFFTs;

    AudioSig *signature = new AudioSig(fEnergy, fAverageZeroCrossing,
                                       fLength, iSpectrum);

cout << fEnergy << endl << fAverageZeroCrossing << endl << fLength << endl;
for (int q = 0; q < 32; q++)
  cout << iSpectrum[q] << endl;

    SigClient *sigClient = new SigClient();
    sigClient->SetAddress("209.249.187.199", 4445);

    if (collID == "")
        collID = "EMPTY_COLLECTION";

    sigClient->GetSignature(signature, strGUID, collID);

    delete pFFT;
    delete signature;
    delete sigClient;

    delete [] m_downmixBuffer;
    m_downmixBuffer = 0;
    m_numSamplesWritten = 0;
}
