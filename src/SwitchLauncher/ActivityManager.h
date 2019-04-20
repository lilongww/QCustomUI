#pragma once

#include <QMap>

#include <memory>

class SWPluginInterface;
class SWContextInterface;
class SWPluginSlots;

class ActivityManager
{
public:
    static ActivityManager& instance();

    bool loadLibarys();
    bool initActivitys(SWContextInterface* context);
    const QMap<int, SWPluginInterface*>& activites() const;
	const QMap<int, SWPluginSlots*>& activitySlots() const;
protected:
    ActivityManager();
    ~ActivityManager();

private:
    struct Impl;
    std::shared_ptr<Impl> m_impl;
};
