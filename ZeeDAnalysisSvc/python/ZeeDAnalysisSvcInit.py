def ZeeDAnalysisSvcInit():

    from AthenaCommon.Logging import logging
    log = logging.getLogger( 'ZeeDAnalysisSvcInit' )

    # get service manager
    from AthenaCommon.AppMgr import ServiceMgr

    # add elec
    if not hasattr(ServiceMgr, 'ZeeDAnalysisSvc' ):
        log.info("Adding ZeeDAnalysisSvc to ServiceMgr")
        from ZeeDAnalysisSvc.ZeeDAnalysisSvcConfig import ZeeDAnalysisSvc
        ServiceMgr += ZeeDAnalysisSvc()
