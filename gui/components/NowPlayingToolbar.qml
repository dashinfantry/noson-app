/*
 * Copyright (C) 2013, 2014, 2015, 2016
 *      Jean-Luc Barriere <jlbarriere68@gmail.com>
 *      Andrew Hayzen <ahayzen@gmail.com>
 *      Daniel Holm <d.holmen@gmail.com>
 *      Victor Thompson <victor.thompson@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Controls 2.2


/* Full toolbar */
Item {
    id: musicToolbarFullContainer
    anchors {
        fill: parent
    }

    property bool mirror: false
    property alias backgroundColor: bg.color
    property alias backgroundOpacity: bg.opacity
    property alias bottomProgressHint: playerControlsProgressBar.visible
    readonly property real toolbarHeight: musicToolbarFullVolumeContainer.height + musicToolbarFullButtonContainer.height

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "transparent"
        opacity: 1.0
    }

    /* volume slider component */
    Item {
        id: musicToolbarFullVolumeContainer
        anchors {
            left: parent.left
            leftMargin: units.gu(2)
            right: parent.right
            rightMargin: units.gu(2)
            top: mirror ? parent.top : musicToolbarFullButtonContainer.bottom
        }
        height: units.gu(6)
        width: parent.width

        RenderingBubble {
            id: renderingBubble
            backgroundColor: "black"
            labelColor: "white"
        }

        /* Mute button */
        Icon {
            id: nowPlayingMuteButton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: units.gu(6)
            width: height
            source: player.mute ? "qrc:/images/audio-volume-muted.svg" : "qrc:/images/audio-volume.svg"
            objectName: "muteShape"
            opacity: 1.0
            onClicked: {
                player.toggleMuteGroup();
                player.refreshRendering();
            }
        }

        StyledSlider {
            id: volumeGroupSlider
            anchors.left: nowPlayingMuteButton.right
            anchors.leftMargin: units.gu(2)
            anchors.rightMargin: units.gu(2)
            anchors.right: nightmodeButton.left
            anchors.verticalCenter: parent.verticalCenter
            wheelEnabled: true
            stepSize: 2.5
            live: true
            from: 0
            to: 100
            objectName: "volumeGroupSliderShape"
            enabled: !player.outputFixed
            opacity: (player.outputFixed ? 0.2 : 1.0)

            handleSize: (player.outputFixed ? 0 : units.gu(2))
            handleColor: styleMusic.playerControls.volumeHandleColor
            handleColorPressed: styleMusic.playerControls.backgroundColor
            handleBorderColor: handleColor
            backgroundColor: styleMusic.playerControls.volumeBackgroundColor
            foregroundColor: styleMusic.playerControls.volumeForegroundColor

            property double inValue

            onValueChanged: {
                if (Math.abs(value - inValue) >= 1.0) {
                    if (pressed && value > inValue + 5.0) {
                        value = inValue + 5.0; // loop on value changed
                    } else {
                        volumeGroupSlider.inValue = player.volumeMaster = Math.round(value);
                        setVolume.start();
                    }
                }
            }

            onPressedChanged: {
                // open the bubble
                if (pressed && player.renderingControlCount > 1)
                    renderingBubble.open(musicToolbarFullVolumeContainer)
            }

            Timer {
                id: setVolume
                interval: 200
                onTriggered: {
                    if (player.setVolumeGroup(volumeGroupSlider.value)) {
                        volumeGroupSlider.inValue = player.volumeMaster = Math.round(volumeGroupSlider.value);
                    } else {
                        customdebug("Set volume failed");
                    }
                }
            }

            Connections {
                target: player
                onVolumeMasterChanged: {
                    // update an icoming change when released only to be smoothest
                    if (!volumeGroupSlider.pressed)
                        volumeGroupSlider.value = volumeGroupSlider.inValue = player.volumeMaster;
                }
            }

            Component.onCompleted: {
                value = inValue = player.volumeMaster;
            }
        }

        /* equalizer button */
        Icon {
            id: nightmodeButton
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: units.gu(5) // smaller
            width: height
            source: "qrc:/images/settings.svg"
            opacity: 1.0
            onClicked: {
                dialogSoundSettings.open()
            }
        }
    }

    Item {
        id: musicToolbarFullButtonContainer
        anchors {
            top: mirror ? musicToolbarFullVolumeContainer.bottom : parent.top
            left: parent.left
            right: parent.right
        }
        height: units.gu(8)
        width: parent.width

        /* Repeat button */
        Rectangle {
            id: nowPlayingRepeatButton
            anchors.right: nowPlayingPreviousButton.left
            anchors.rightMargin: units.gu(1)
            anchors.verticalCenter: nowPlayingPlayButton.verticalCenter
            height: units.gu(6)
            width: height
            color: "transparent"
            visible: player.isPlayingQueued()

            Icon {
                id: repeatIcon
                height: units.gu(5)
                width: height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: styleMusic.playerControls.labelColor
                source: "qrc:/images/media-playlist-repeat.svg"
                objectName: "repeatShape"
                opacity: player.repeat ? 1 : .3
                onClicked: {
                    var old = player.repeat
                    if (player.toggleRepeat())
                        player.repeat = !old
                }
            }
        }

        /* Previous button */
        Rectangle {
            id: nowPlayingPreviousButton
            anchors.right: nowPlayingPlayButton.left
            anchors.rightMargin: units.gu(1)
            anchors.verticalCenter: nowPlayingPlayButton.verticalCenter
            height: units.gu(6)
            width: height
            color: "transparent"
            visible: player.isPlayingQueued()

            Icon {
                id: nowPlayingPreviousIndicator
                height: units.gu(5)
                width: height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: styleMusic.playerControls.labelColor
                source: "qrc:/images/media-skip-backward.svg"
                opacity: player.trackQueue.model.count > 0 && player.currentIndex > 0  ? 1 : .3
                onClicked: player.previousSong()
            }
        }

        /* Play/Pause button */
        Rectangle {
            id: nowPlayingPlayButton
            anchors.centerIn: parent
            height: units.gu(8)
            width: height
            color: "transparent"

            Icon {
                id: nowPlayingPlayIndicator
                height: units.gu(8)
                width: height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: styleMusic.playerControls.labelColor
                source: player.isPlaying ? "qrc:/images/media-playback-pause.svg" : "qrc:/images/media-playback-start.svg"
                onClicked: player.toggle()
            }
        }

        /* Next button */
        Rectangle {
            id: nowPlayingNextButton
            anchors.left: nowPlayingPlayButton.right
            anchors.leftMargin: units.gu(1)
            anchors.verticalCenter: nowPlayingPlayButton.verticalCenter
            height: units.gu(6)
            width: height
            color: "transparent"
            visible: player.isPlayingQueued()

            Icon {
                id: nowPlayingNextIndicator
                height: units.gu(5)
                width: height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: styleMusic.playerControls.labelColor
                source: "qrc:/images/media-skip-forward.svg"
                objectName: "forwardShape"
                opacity: player.trackQueue.model.count > 0 && (player.currentIndex + 1) < player.trackQueue.model.count ? 1 : .3
                onClicked: player.nextSong()
            }
        }

        /* Shuffle button */
        Rectangle {
            id: nowPlayingShuffleButton
            anchors.left: nowPlayingNextButton.right
            anchors.leftMargin: units.gu(1)
            anchors.verticalCenter: nowPlayingPlayButton.verticalCenter
            height: units.gu(6)
            width: height
            color: "transparent"
            visible: player.isPlayingQueued()

            Icon {
                id: shuffleIcon
                height: units.gu(5)
                width: height
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color: styleMusic.playerControls.labelColor
                source: "qrc:/images/media-playlist-shuffle.svg"
                objectName: "shuffleShape"
                opacity: player.shuffle ? 1 : .3
                onClicked: {
                    var old = player.shuffle
                    if (player.toggleShuffle())
                        player.shuffle = !old
                }
            }
        }
    }

    /* Object which provides the progress bar when in the queue */
    Rectangle {
        id: playerControlsProgressBar
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        color: "transparent"
        height: units.gu(0.25)
        visible: isListView && player.isPlayingQueued()

        Rectangle {
            id: playerControlsProgressBarHint
            anchors {
                left: parent.left
                bottom: parent.bottom
            }
            color: styleMusic.playerControls.progressForegroundColor
            height: parent.height
            width: hint(player.trackPosition, player.trackDuration)

            function hint(position, duration) {
                var val = 0;
                if (position && duration)
                    val = (duration > 0 ? (position / duration) * playerControlsProgressBar.width : 0);
                playerControlsProgressBarHint.width = val;
            }

            Connections {
                target: player
                onCurrentPositionChanged: playerControlsProgressBarHint.hint(position, duration)
                onStopped: playerControlsProgressBarHint.hint(0, player.trackDuration)
            }
        }
    }
}
