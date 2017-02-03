C---------------------------------------------------------------------

      subroutine PhotosInit()

C     Inicialisation of photos interface     
      INTEGER  d_h_nmxhep             ! maximum number of particles
      PARAMETER (d_h_NMXHEP=10000)

      REAL*8  d_h_phep,  d_h_vhep ! to be real*4/ *8  depending on host
      INTEGER d_h_nevhep,d_h_nhep,d_h_isthep,d_h_idhep,d_h_jmohep,
     $        d_h_jdahep
      COMMON /hepevt/

     $      d_h_nevhep,               ! serial number
     $      d_h_nhep,                 ! number of particles
     $      d_h_isthep(d_h_nmxhep),   ! status code
     $      d_h_idhep(d_h_nmxhep),    ! particle ident KF
     $      d_h_jmohep(2,d_h_nmxhep), ! parent particles
     $      d_h_jdahep(2,d_h_nmxhep), ! childreen particles
     $      d_h_phep(5,d_h_nmxhep),   ! four-momentum, mass [GeV]
     $      d_h_vhep(4,d_h_nmxhep)    ! vertex [mm]
* ----------------------------------------------------------------------
      
      CALL phoini(1)

      Write (*,*) "--------------------------------------------------"
      Write (*,*) "--  inicialization of photos package is OK      --"      
      Write (*,*) "--------------------------------------------------"

      RETURN
      END
C---------------------------------------------------------------------

      Subroutine SetPhotosParam(
     & in_z,
     & in_elecm,
     & in_elecp
     &)
C-------------------------------------
      real*8 in_z(5), in_elecm(5), in_elecp(5)
      INTEGER  d_h_nmxhep             ! maximum number of particles
      PARAMETER (d_h_NMXHEP=10000)

      REAL*8  d_h_phep,  d_h_vhep ! to be real*4/ *8  depending on host
      INTEGER d_h_nevhep,d_h_nhep,d_h_isthep,d_h_idhep,d_h_jmohep,
     $        d_h_jdahep
      COMMON /hepevt/

     $      d_h_nevhep,               ! serial number
     $      d_h_nhep,                 ! number of particles
     $      d_h_isthep(d_h_nmxhep),   ! status code
     $      d_h_idhep(d_h_nmxhep),    ! particle ident KF
     $      d_h_jmohep(2,d_h_nmxhep), ! parent particles
     $      d_h_jdahep(2,d_h_nmxhep), ! childreen particles
     $      d_h_phep(5,d_h_nmxhep),   ! four-momentum, mass [GeV]
     $      d_h_vhep(4,d_h_nmxhep)    ! vertex [mm]
* ----------------------------------------------------------------------
      integer icount
      data icount /0/
      save icount
C-------------------------------------
      icount = icount + 1
      
      d_h_nevhep = icount
      d_h_nhep   = 3

      d_h_isthep(1) = 2
      d_h_isthep(2) = 1   
      d_h_isthep(3) = 1

      d_h_idhep(1) = 23
      d_h_idhep(2) = 11
      d_h_idhep(3) = -11


      d_h_jmohep(1,2) = 1
      d_h_jmohep(1,3) = 1

      d_h_jmohep(2,2) = 1
      d_h_jmohep(2,3) = 1

      d_h_jmohep(1,1) = 0
      d_h_jmohep(2,1) = 0

      d_h_jdahep(1,1) = 2
      d_h_jdahep(2,1) = 3

      d_h_jdahep(1,2) = 0
      d_h_jdahep(1,3) = 0
      d_h_jdahep(2,2) = 0
      d_h_jdahep(2,3) = 0

      
      do i=1,5 
          d_h_phep(i,1) = in_z(i)
          d_h_phep(i,2) = in_elecm(i)
          d_h_phep(i,3) = in_elecp(i)
      enddo


      do i=1,4
         d_h_vhep(i,1) = 0
         d_h_vhep(i,2) = 0
         d_h_vhep(i,3) = 0
      enddo


      RETURN
      END

C---------------------------------------------------------------------

      Subroutine GetPhotos()


C-------------------------------------
      INTEGER  d_h_nmxhep             ! maximum number of particles
      PARAMETER (d_h_NMXHEP=10000)

      REAL*8  d_h_phep,  d_h_vhep ! to be real*4/ *8  depending on host
      INTEGER d_h_nevhep,d_h_nhep,d_h_isthep,d_h_idhep,d_h_jmohep,
     $        d_h_jdahep
      COMMON /hepevt/

     $      d_h_nevhep,               ! serial number
     $      d_h_nhep,                 ! number of particles
     $      d_h_isthep(d_h_nmxhep),   ! status code
     $      d_h_idhep(d_h_nmxhep),    ! particle ident KF
     $      d_h_jmohep(2,d_h_nmxhep), ! parent particles
     $      d_h_jdahep(2,d_h_nmxhep), ! childreen particles
     $      d_h_phep(5,d_h_nmxhep),   ! four-momentum, mass [GeV]
     $      d_h_vhep(4,d_h_nmxhep)    ! vertex [mm]
* -------------------------------------------------------------------
      CALL Photos(1)

C     Print*,  'Number of particles after photos' , d_h_nhep
      end
C--------------------------------------------------------------------



      Subroutine GetPhotosParams( 
     & out_z,
     & out_elecm,
     & out_elecp,
     & out_phot1,
     & out_phot2
     &)


C-------------------------------------
      real*8 out_z(5), out_elecm(5), out_elecp(5)
      real*8 out_phot1(5), out_phot2(5)

      INTEGER  d_h_nmxhep             ! maximum number of particles
      PARAMETER (d_h_NMXHEP=10000)

      REAL*8  d_h_phep,  d_h_vhep ! to be real*4/ *8  depending on host
      INTEGER d_h_nevhep,d_h_nhep,d_h_isthep,d_h_idhep,d_h_jmohep,
     $        d_h_jdahep
      COMMON /hepevt/

     $      d_h_nevhep,               ! serial number
     $      d_h_nhep,                 ! number of particles
     $      d_h_isthep(d_h_nmxhep),   ! status code
     $      d_h_idhep(d_h_nmxhep),    ! particle ident KF
     $      d_h_jmohep(2,d_h_nmxhep), ! parent particles
     $      d_h_jdahep(2,d_h_nmxhep), ! childreen particles
     $      d_h_phep(5,d_h_nmxhep),   ! four-momentum, mass [GeV]
     $      d_h_vhep(4,d_h_nmxhep)    ! vertex [mm]
* ----------------------------------------------------------------------

      do i=1,5 
          out_z(i)     = d_h_phep(i,1) 
          out_elecm(i) = d_h_phep(i,2) 
          out_elecp(i) = d_h_phep(i,3)
          out_phot1(i) = 0
          out_phot2(i) = 0

          if(d_h_nhep > 3) then
          out_phot1(i) = d_h_phep(i,3)
C         else out_phot1(i) = 0
          end if

          if(d_h_nhep > 4) then
          out_phot2(i) = d_h_phep(i,3)
C         else out_phot2(i) = 0
          end if

      enddo


      END
