#ifndef StRPEvent_h
#define StRPEvent_h

//_____________________________________________________________________________
//    Class for UPC data
//    Author: Truhlar Tomas
//_____________________________________________________________________________


class StUPCRpsCollection; 
class StUPCRpsRomanPot; 
class StUPCRpsPlane; 
class StUPCRpsCluster; 
class StUPCRpsTrack;
class StUPCRpsTrackPoint;

class TClonesArray;
class TIterator;
class TParticle;

#include "TArrayI.h"

class StRPEvent
{

public:

  StRPEvent();
  virtual ~StRPEvent(); 

  void clearEvent(); 

  void addTriggerId(Int_t id);


  //setters
  StUPCRpsCollection *addCollection(); 
  StUPCRpsTrack *addTrack();
  StUPCRpsTrackPoint *addTrackPoint();

  //getters
  Bool_t isTrigger(Int_t id) const;

  Int_t getNumberOfTracks() const;
  StUPCRpsTrack *getTrack(Int_t iTrack) const;
  Int_t getNumberOfTrackPoints() const;
  StUPCRpsTrackPoint *getTrackPoint(Int_t iTrackPoint) const;

  Int_t getNumberOfCollections() const;
  StUPCRpsCollection *getCollection(Int_t iCollection) const;

private:

  StRPEvent (const StRPEvent &o); 
  StRPEvent &operator=(const StRPEvent &o); 

  TArrayI mTrgIDs; // fired trigger IDs

  static TClonesArray *mgTracks; // array of upc tracks
  TClonesArray *mTracks; //-> array of upc tracks
  Int_t mNTracks; //! number of upc tracks in event, local use when filling

  static TClonesArray *mgTrackPoints; // array of upc tracks
  TClonesArray *mTrackPoints; //-> array of upc tracks
  Int_t mNTrackPoints; //! number of upc tracks in event, local use when filling

  static TClonesArray *mgCollections; // array of UPC vertices
  TClonesArray *mCollections; //-> array of UPC vertices
  Int_t mNCollections; //! number of vertices written in event, local use when filling


  ClassDef(StRPEvent, 2);
};

#endif













