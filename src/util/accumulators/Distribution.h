#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

/*
* Simpatico - Simulation Package for Polymeric and Molecular Liquids
*
* Copyright 2010, David Morse (morse@cems.umn.edu)
* Distributed under the terms of the GNU General Public License.
*/

#include <util/param/ParamComposite.h>  // base class
#include <util/containers/DArray.h>     // member template

namespace Util
{

   /**
   * A distribution (or histogram) of values for a real variable.
   *
   * \ingroup Accumulators_Module
   */
   class Distribution : public ParamComposite 
   {
   
   public:
  
      /** 
      * Default constructor.
      */
      Distribution();
   
      /** 
      * Copy constructor.
      *
      * \param other Distribution to be copied.
      */
      Distribution(const Distribution& other);
   
      /** 
      * Assignment operator.
      *
      * \param other Distribution to be assigned.
      */
      Distribution& operator = (const Distribution& other);
   
      /** 
      * Destructor.
      */
      virtual ~Distribution();
   
      /**
      * Read parameters from file and initialize.
      *
      * Read values of min, max, and nBin from file.
      * Allocate histogram array and clear all accumulators.
      *
      * \param in input parameter file stream
      */
      virtual void readParam(std::istream& in);
  
      /** 
      * Set parameters and initialize.
      *
      * \param min  lower bound of range
      * \param max  upper bound of range
      * \param nBin number of bins in range [min, max]
      */
      void setParam(double min, double max, int nBin);
   
      /**
      * Clear (i.e., zero) previously allocated histogram.
      */
      virtual void clear();
   
      /**
      * Sample a value.
      *
      * \param value current value
      */
      void sample(double value);
   
      /**
      * Output the distribution to file. 
      *
      * \param out output stream
      */
      void output(std::ostream& out);
  
      /**
      * Return the index of the bin for a value.
      *
      * \param value sampled value
      */
      int binIndex(double value);
  
      /** 
      * Get minimum value in range of histogram.
      */
      double min() const;
  
      /** 
      * Get maximum value in range of histogram.
      */
      double max() const;
    
      /** 
      * Get binWidth, the width of each bin.
      */
      double binWidth() const;
  
      /** 
      * Get the number of bins
      */
      int nBin() const;
        
      /**
      * Serialize this Distribution to/from an archive.
      *
      * \param ar       input or output archive
      * \param version  file version id
      */
      template <class Archive>
      void serialize(Archive& ar, const unsigned int version);

   protected:
   
      DArray<long> histogram_;  ///< Histogram of occurences, one element per bin.
      double  min_;             ///< minimum value.
      double  max_;             ///< maximum value.
      double  binWidth_;        ///< width of bin = (max_-min_)/nBin_ .
      int     nBin_;            ///< number of bins.
      int     nSample_;         ///< Number of sampled values in Histogram.
      int     nReject_;         ///< Number of sampled values that were out of range.
   
   };

   // inline method definitions 
  
   /*
   * Return the index of the bin for a value.
   */
   inline int Distribution::binIndex(double value) 
   { return int( (value - min_)/binWidth_ ); }
  
   /*
   * Get minimum value in range of histogram.
   */
   inline double Distribution::min() const
   {  return min_; }
  
   /*
   * Get maximum value in range of histogram.
   */
   inline double Distribution::max() const
   {  return max_; }
 
   /*
   * Get binWidth, the width of each bin.
   */
   inline double Distribution::binWidth() const
   {  return binWidth_; }
  
   /*
   * Get the number of bins
   */
   inline int Distribution::nBin() const
   {  return nBin_; }

   /*
   * Serialize this Distribution.
   */
   template <class Archive>
   void Distribution::serialize(Archive& ar, const unsigned int version)
   {
      ar & histogram_; 
      ar & min_;        
      ar & max_;    
      ar & binWidth_;  
      ar & nBin_;     
      ar & nSample_;   
      ar & nReject_;    
   }
        
}
#endif