#ifndef PFC_WATCHDOG_H
#define PFC_WATCHDOG_H

extern "C" {
#include "sai.h"
}

#include <atomic>
#include <vector>
#include <condition_variable>
#include "swss/table.h"

class PfcWatchdog
{
    public:
        static void setPortCounterList(
                _In_ sai_object_id_t queue,
                _In_ const std::vector<sai_port_stat_t> &counterIds);
        static void setQueueCounterList(
                _In_ sai_object_id_t queue,
                _In_ const std::vector<sai_queue_stat_t> &counterIds);
        static void removeQueue(
                _In_ sai_object_id_t queue);

        PfcWatchdog(
                _In_ const PfcWatchdog&) = delete;
        ~PfcWatchdog(void);

    private:
        struct PfcCounterIds
        {
            PfcCounterIds(
                    _In_ sai_object_id_t,
                    _In_ const std::vector<sai_port_stat_t> &portIds,
                    _In_ const std::vector<sai_queue_stat_t> &queueIds);

            sai_object_id_t portId;
            std::vector<sai_port_stat_t> portCounterIds;
            std::vector<sai_queue_stat_t> queueCounterIds;
        };

        PfcWatchdog(void);
        static PfcWatchdog& getInstance(void);
        static sai_object_id_t queueIdToPortId(
                _In_ sai_object_id_t queueId);
        void collectCounters(
                _In_ swss::Table &countersTable);
        void pfcWatchdogThread(void);
        void startWatchdogThread(void);
        void endWatchdogThread(void);

        std::map<sai_object_id_t, std::shared_ptr<PfcCounterIds>> m_counterIdsMap;
        std::mutex m_mutex;

        std::atomic_bool m_runPfcWatchdogThread = { false };
        std::shared_ptr<std::thread> m_pfcWatchdogThread = nullptr;
        std::mutex m_mtxSleep;
        std::condition_variable m_cvSleep;
};

#endif
