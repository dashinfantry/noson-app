/*
 *      Copyright (C) 2015-2019 Jean-Luc Barriere
 *
 *  This file is part of Noson-App
 *
 *  Noson is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Noson is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Noson.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NOSONAPPPLAYER_H
#define NOSONAPPPLAYER_H

#include <noson/sonosplayer.h>

#include "queuemodel.h"

#include <QObject>

namespace nosonapp
{

class Sonos;
class Mpris2;

class Player : public QObject, public ContentProvider<Player>
{
  Q_OBJECT
  Q_PROPERTY(bool muteMaster READ muteMaster NOTIFY renderingGroupChanged)
  Q_PROPERTY(bool nightmode READ nightmode NOTIFY renderingGroupChanged)
  Q_PROPERTY(bool loudness READ loudness NOTIFY renderingGroupChanged)
  Q_PROPERTY(bool outputFixed READ outputFixed NOTIFY renderingGroupChanged)
  Q_PROPERTY(int volumeMaster READ volumeMaster NOTIFY renderingGroupChanged)
  Q_PROPERTY(int treble READ treble NOTIFY renderingGroupChanged)
  Q_PROPERTY(int bass READ bass NOTIFY renderingGroupChanged)

  // Read only
  Q_PROPERTY(int pid READ pid NOTIFY connectedChanged)
  Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
  Q_PROPERTY(QString zoneId READ zoneId NOTIFY connectedChanged)
  Q_PROPERTY(QString zoneName READ zoneName NOTIFY connectedChanged)
  Q_PROPERTY(QString zoneShortName READ zoneShortName NOTIFY connectedChanged)
  Q_PROPERTY(QString controllerURI READ controllerURI NOTIFY connectedChanged)
  Q_PROPERTY(QString currentMetaAlbum READ currentMetaAlbum NOTIFY sourceChanged)
  Q_PROPERTY(QString currentMetaArt READ currentMetaArt NOTIFY sourceChanged)
  Q_PROPERTY(QString currentMetaArtist READ currentMetaArtist NOTIFY sourceChanged)
  Q_PROPERTY(QString currentMetaSource READ currentMetaSource NOTIFY sourceChanged)
  Q_PROPERTY(QString currentMetaTitle READ currentMetaTitle NOTIFY sourceChanged)
  Q_PROPERTY(QString currentMetaURITitle READ currentMetaURITitle NOTIFY sourceChanged)
  Q_PROPERTY(int currentIndex READ currentIndex NOTIFY sourceChanged)
  Q_PROPERTY(int currentTrackDuration READ currentTrackDuration NOTIFY sourceChanged)
  Q_PROPERTY(int currentProtocol READ currentProtocol NOTIFY sourceChanged)

  Q_PROPERTY(int numberOfTracks READ numberOfTracks NOTIFY playbackStateChanged)
  Q_PROPERTY(QString playbackState READ playbackState NOTIFY playbackStateChanged)
  Q_PROPERTY(QString playMode READ playMode NOTIFY playModeChanged)

public:
  explicit Player(QObject *parent = nullptr);
  ~Player();

  Q_INVOKABLE bool init(Sonos* sonos, const QString& zoneName);
  Q_INVOKABLE bool init(Sonos* sonos, const QVariant& zone);

  bool init(Sonos* sonos, const SONOS::ZonePtr& zone);
  bool connected() const { return m_connected; }
  QString controllerURI() const { return m_controllerURI; }
  void beginJob();
  void endJob();

  SONOS::ZonePtr zone() const;
  int pid() const { return m_pid; }
  void setPid(int pid) { m_pid = pid; }

  Q_INVOKABLE void enableMPRIS2();
  Q_INVOKABLE void disableMPRIS2();

  Q_INVOKABLE bool ping();
  Q_INVOKABLE QString zoneId() const;
  Q_INVOKABLE QString zoneName() const;
  Q_INVOKABLE QString zoneShortName() const;
  Q_INVOKABLE QString coordinatorName() const;

  Q_INVOKABLE bool configureSleepTimer(int seconds);
  Q_INVOKABLE int remainingSleepTimerDuration();

  Q_INVOKABLE bool startPlaySource(const QVariant& payload); // asynchronous
  Q_INVOKABLE bool play();
  Q_INVOKABLE bool stop();
  Q_INVOKABLE bool pause();
  Q_INVOKABLE bool previous();
  Q_INVOKABLE bool next();
  Q_INVOKABLE bool toggleRepeat();
  Q_INVOKABLE bool toggleShuffle();
  Q_INVOKABLE bool setSource(const QVariant& payload);
  Q_INVOKABLE bool toggleMute();
  Q_INVOKABLE bool toggleMute(const QString& uuid);

  Q_INVOKABLE bool toggleNightmode();
  Q_INVOKABLE bool toggleNightmode(const QString& uuid);
  Q_INVOKABLE bool toggleLoudness();
  Q_INVOKABLE bool toggleLoudness(const QString& uuid);
  Q_INVOKABLE bool toggleOutputFixed(const QString& uuid);
  Q_INVOKABLE bool supportsOutputFixed(const QString& uuid);

  Q_INVOKABLE bool startPlayStream(const QString& url, const QString& title); // asynchonous
  Q_INVOKABLE bool playStream(const QString& url, const QString& title);

  Q_INVOKABLE bool playPulse();
  Q_INVOKABLE bool isPulseStream(const QString& url); // return true if the given url is pulse stream from this player

  Q_INVOKABLE bool isMyStream(const QString& url);    // return true if the given url connects to this player

  Q_INVOKABLE QString makeFilePictureURL(const QString& filePath);
  Q_INVOKABLE QString makeFilePictureLocalURL(const QString& filePath);
  Q_INVOKABLE QVariant makeFileStreamItem(const QString& filePath,
                                          const QString& codec,
                                          const QString& title,
                                          const QString& album,
                                          const QString& author,
                                          const QString& duration,
                                          bool hasArt);

  Q_INVOKABLE bool playLineIN();
  Q_INVOKABLE bool playDigitalIN();
  Q_INVOKABLE bool playQueue(bool start);
  Q_INVOKABLE bool seekTime(int timesec);
  Q_INVOKABLE bool seekTrack(int position);
  Q_INVOKABLE int addItemToQueue(const QVariant& payload, int position);
  Q_INVOKABLE int addMultipleItemsToQueue(const QVariantList& payloads);
  Q_INVOKABLE bool removeAllTracksFromQueue();
  Q_INVOKABLE bool removeTrackFromQueue(const QString& id, int containerUpdateID);
  Q_INVOKABLE bool reorderTrackInQueue(int trackNo, int newPosition, int containerUpdateID);

  Q_INVOKABLE bool saveQueue(const QString& title);
  Q_INVOKABLE bool createSavedQueue(const QString& title);
  Q_INVOKABLE int addItemToSavedQueue(const QString& SQid, const QVariant& payload, int containerUpdateID);
  Q_INVOKABLE bool removeTracksFromSavedQueue(const QString& SQid, const QVariantList& indexes, int containerUpdateID);
  Q_INVOKABLE bool reorderTrackInSavedQueue(const QString& SQid, int index, int newIndex, int containerUpdateID);

  Q_INVOKABLE bool startPlayFavorite(const QVariant& payload); // asynchronous
  Q_INVOKABLE bool playFavorite(const QVariant& payload);

  bool nightmode() const { return m_RCGroup.nightmode; }
  bool loudness() const { return m_RCGroup.loudness; }
  bool muteMaster() const { return m_RCGroup.mute; }
  bool outputFixed() const { return m_RCGroup.outputFixed; }
  int muteLF() const { return m_RCGroup.mute; }
  int muteRF() const { return m_RCGroup.mute; }
  int volumeMaster() const { return m_RCGroup.volume; }
  int volumeLF() const { return m_RCGroup.volume; }
  int volumeRF() const { return m_RCGroup.volume; }
  int treble() const { return m_RCGroup.treble; }
  int bass() const { return m_RCGroup.bass; }

  struct RCProperty
  {
    std::string uuid;
    std::string name;
    bool mute = false;
    bool nightmode = false;
    bool loudness = false;
    bool outputFixed = false;
    int volume = 0;
    int treble = 0;
    int bass = 0;
    double volumeFake = 0.;
  };

  typedef std::vector<RCProperty> RCTable;

  const RCTable& renderingTable() const { return m_RCTable; }

  Q_INVOKABLE bool setTreble(double val);
  Q_INVOKABLE bool setBass(double val);

  Q_INVOKABLE bool setVolumeGroup(double volume);
  Q_INVOKABLE bool setVolume(const QString& uuid, double volume);

  Q_INVOKABLE int currentTrackPosition();

  void setCurrentMeta(const SONOS::AVTProperty& prop);
  const QString& currentMetaAlbum() const { return m_currentMetaAlbum; }
  const QString& currentMetaArt() const { return m_currentMetaArt; }
  const QString& currentMetaArtist() const { return m_currentMetaArtist; }
  const QString& currentMetaSource() const { return m_currentMetaSource; }
  const QString& currentMetaTitle() const { return m_currentMetaTitle; }
  const QString& currentMetaURITitle() const { return m_currentMetaURITitle; }
  int currentIndex() const { return m_currentIndex; }
  int currentTrackDuration() const { return m_currentTrackDuration; }
  int currentProtocol() const { return m_currentProtocol; } // returns SONOS::Protocol_t
  int numberOfTracks() const { return m_AVTProperty.NumberOfTracks; }
  QString playbackState() const { return QString::fromUtf8(m_AVTProperty.TransportState.c_str()); }
  QString playMode() const { return QString::fromUtf8(m_AVTProperty.CurrentPlayMode.c_str()); }

  // Implements ContentProvider
  void beforeLoad();
  void afterLoad();
  void runContentLoader(ListModel<Player>* model);
  void loadContent(ListModel<Player>* model);
  void loadAllContent();
  void runContentLoaderForContext(ListModel<Player>* model, int id);
  void loadContentForContext(ListModel<Player>* model, int id);
  const char* getHost() const;
  unsigned getPort() const;
  QString getBaseUrl() const;
  void registerContent(ListModel<Player>* model, const QString& root);
  void unregisterContent(ListModel<Player>* model);

signals:
  void jobFailed(int pid);
  void connectedChanged(int pid);
  void renderingChanged(int pid);
  void renderingGroupChanged(int pid);
  void sourceChanged(int pid);
  void playbackStateChanged(int pid);
  void playModeChanged(int pid);
  void sleepTimerChanged(int pid);

private slots:
    void handleTransportChange();
    void handleRenderingControlChange();

private:
  int m_pid;
  Sonos* m_sonos;
  SONOS::PlayerPtr m_player;
  SONOS::AVTProperty m_AVTProperty;

  RCTable m_RCTable; // Rendering control of subordinates
  RCProperty m_RCGroup; // Rendering control of the group

  bool m_connected;
  QString m_controllerURI; // Filled with the public URI of this controller when connected

  QString m_currentMetaAlbum;
  QString m_currentMetaArt;
  QString m_currentMetaArtist;
  QString m_currentMetaSource;
  QString m_currentMetaTitle;
  QString m_currentMetaURITitle;
  int m_currentIndex;
  int m_currentTrackDuration;
  int m_currentProtocol;

  typedef RegisteredContent<Player> ManagedQueue;
  Locked<ManagedQueue> m_queue;
  unsigned m_queueUpdateID;

  static void playerEventCB(void* handle);

  Mpris2* m_mpris2;
};

}

#endif // NOSONAPPPLAYER_H
