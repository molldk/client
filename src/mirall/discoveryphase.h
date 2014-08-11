/*
 * Copyright (C) by Olivier Goffart <ogoffart@woboq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */

#pragma once

#include <QObject>
#include <QStringList>
#include <csync.h>

/**
 * The Discovery Phase was once called "update" phase in csync therms.
 * Its goal is to look at the files in one of the remote and check comared to the db
 * if the files are new, or changed.
 */

class DiscoveryJob : public QObject {
    Q_OBJECT
    CSYNC *_csync_ctx;
    csync_log_callback _log_callback;
    int _log_level;
    void* _log_userdata;

    /**
     * return true if the given path should be synced,
     * false if the path should be ignored
     */
    bool isInWhiteList(const QString &path) const;
    static int isInWhiteListCallBack(void *, const char *);

public:
    explicit DiscoveryJob(CSYNC *ctx, QObject* parent = 0)
            : QObject(parent), _csync_ctx(ctx) {
        // We need to forward the log property as csync uses thread local
        // and updates run in another thread
        _log_callback = csync_get_log_callback();
        _log_level = csync_get_log_level();
        _log_userdata = csync_get_log_userdata();
    }

    QStringList _selectiveSyncWhiteList;
    Q_INVOKABLE void start();
signals:
    void finished(int result);
};
