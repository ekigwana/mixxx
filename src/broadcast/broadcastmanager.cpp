#include "broadcast/broadcastmanager.h"

#include "broadcast/defs_broadcast.h"
#include "engine/enginemaster.h"
#include "engine/sidechain/enginenetworkstream.h"
#include "engine/sidechain/enginesidechain.h"
#include "soundio/soundmanager.h"

BroadcastManager::BroadcastManager(SettingsManager* pSettingsManager,
                                   SoundManager* pSoundManager)
        : m_pConfig(pSettingsManager->settings()) {
    QSharedPointer<EngineNetworkStream> pNetworkStream =
            pSoundManager->getNetworkStream();

    if (pNetworkStream) {
        m_pBroadcast = QSharedPointer<EngineBroadcast>(
                new EngineBroadcast(m_pConfig,
                                    pSettingsManager->broadcastSettings(),
                                    pNetworkStream->getNumOutputChannels()));
        pNetworkStream->addWorker(m_pBroadcast);
    }

    m_pBroadcastEnabled = new ControlProxy(
            BROADCAST_PREF_KEY, "enabled", this);
    m_pBroadcastEnabled->connectValueChanged(SLOT(slotControlEnabled(double)));
}

BroadcastManager::~BroadcastManager() {
    // Disable broadcast so when Mixxx starts again it will not connect.
    m_pBroadcastEnabled->set(0);
}

void BroadcastManager::setEnabled(bool value) {
    m_pBroadcastEnabled->set(value);
}

bool BroadcastManager::isEnabled() {
    return m_pBroadcastEnabled->toBool();
}

void BroadcastManager::slotControlEnabled(double v) {
    emit(broadcastEnabled(v > 0.0));
}
