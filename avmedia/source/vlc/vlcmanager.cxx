/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <com/sun/star/uno/Exception.hpp>
#include <cppuhelper/supportsservice.hxx>
#include <sal/log.hxx>
#include "vlcmanager.hxx"
#include "vlcplayer.hxx"
#include <wrapper/Instance.hxx>
#include <wrapper/EventManager.hxx>
#include <wrapper/Media.hxx>
#include <wrapper/Player.hxx>
#include <wrapper/Common.hxx>

using namespace ::com::sun::star;

namespace avmedia::vlc {

namespace
{
    const OUString VLC_IMPLEMENTATION_NAME = "com.sun.star.comp.avmedia.Manager_VLC";
    const OUString VLC_SERVICENAME = "com.sun.star.media.Manager_VLC";

    const char * const VLC_ARGS[] = {
        "--demux",
        "ffmpeg",
        "--no-mouse-events",
        "--verbose=-1"
    };
}

Manager::Manager()
    : mEventHandler()
{
    using namespace wrapper;
    static bool success = Instance::LoadSymbols() && EventManager::LoadSymbols()
                          && Media::LoadSymbols() && Player::LoadSymbols()
                          && Common::LoadSymbols();

    m_is_vlc_found = success;
    if (m_is_vlc_found)
    {
        mInstance.reset(new Instance( SAL_N_ELEMENTS(VLC_ARGS), VLC_ARGS ));
        //Check VLC version
        std::vector<std::string> verComponents;
        const std::string str(Common::Version());

        boost::split(verComponents,
                     str,
                     boost::is_any_of(". "));
        if (verComponents.size() < 3
            || boost::lexical_cast<int>(verComponents[0]) < 2
            || (boost::lexical_cast<int>(verComponents[1]) == 0
                && boost::lexical_cast<int>(verComponents[2]) < 8))
        {
            SAL_WARN("avmedia", "VLC version '" << str << "' is too old");
            m_is_vlc_found = false;
        }
        else
            SAL_INFO("avmedia", "VLC version '" << str << "' is acceptable");
    }
    else
        SAL_WARN("avmedia", "Cannot load symbols");

    if (m_is_vlc_found)
    {
        mEventHandler.create();
    }
}

Manager::~Manager()
{
    mEventHandler.stop();
}

uno::Reference< media::XPlayer > SAL_CALL Manager::createPlayer( const OUString& rURL )
{
    if ( !m_is_vlc_found )
        throw uno::RuntimeException("VLC not found", nullptr);

    if ( !rURL.isEmpty() )
    {
        if (mURL == rURL)
            return mPlayer;

        mURL = rURL;
    }
    else
        return mPlayer;

    VLCPlayer* pPlayer( new VLCPlayer( mURL,
                                       *mInstance,
                                       mEventHandler /*, mxMgr */ ) );
    mPlayer.set( pPlayer );

    return mPlayer;
}

OUString SAL_CALL Manager::getImplementationName()
{
    return VLC_IMPLEMENTATION_NAME;
}

sal_Bool SAL_CALL Manager::supportsService( const OUString& serviceName )
{
    return cppu::supportsService(this, serviceName);
}

uno::Sequence< OUString > SAL_CALL Manager::getSupportedServiceNames()
{
    return { VLC_SERVICENAME };
}

} // end namespace

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
