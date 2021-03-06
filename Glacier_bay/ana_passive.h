      SUBROUTINE ana_passive (ng, tile, model)
!
!! svn $Id$
!!======================================================================
!! Copyright (c) 2002-2017 The ROMS/TOMS Group                         !
!!   Licensed under a MIT/X style license                              !
!!   See License_ROMS.txt                                              !
!!                                                                     !
!=======================================================================
!                                                                      !
!  This routine sets initial conditions for passive inert tracers      !
!  using analytical expressions.                                       !
!                                                                      !
!=======================================================================
!
      USE mod_param
      USE mod_ncparam
      USE mod_ocean
!
! Imported variable declarations.
!
      integer, intent(in) :: ng, tile, model

#include "tile.h"
!
      CALL ana_passive_tile (ng, tile, model,                           &
     &                       LBi, UBi, LBj, UBj,                        &
     &                       IminS, ImaxS, JminS, JmaxS,                &
     &                       OCEAN(ng) % t)
!
! Set analytical header file name used.
!
#ifdef DISTRIBUTE
      IF (Lanafile) THEN
#else
      IF (Lanafile.and.(tile.eq.0)) THEN
#endif
        ANANAME(18)=__FILE__
      END IF

      RETURN
      END SUBROUTINE ana_passive
!
!***********************************************************************
      SUBROUTINE ana_passive_tile (ng, tile, model,                     &
     &                             LBi, UBi, LBj, UBj,                  &
     &                             IminS, ImaxS, JminS, JmaxS,          &
     &                             t)
!***********************************************************************
!
      USE mod_grid
      USE mod_param
      USE mod_scalars
!
!  Imported variable declarations.
!
      integer, intent(in) :: ng, tile, model
      integer, intent(in) :: LBi, UBi, LBj, UBj
      integer, intent(in) :: IminS, ImaxS, JminS, JmaxS
!
#ifdef ASSUMED_SHAPE
      real(r8), intent(out) :: t(LBi:,LBj:,:,:,:)
#else
      real(r8), intent(out) :: t(LBi:UBi,LBj:UBj,N(ng),3,NT(ng))
#endif
!
!  Local variable declarations.
!
      integer :: i, ip, itrc, j, k

#include "set_bounds.h"
!
!-----------------------------------------------------------------------
!  Set analytical initial conditions for passive inert tracers.
!-----------------------------------------------------------------------
! Was for Arctic
!     DO k=1,N(ng)
!       DO j=JstrT,JendT
!         DO i=IstrT,IendT
!           t(i,j,k,1,inert(1)) = i
!           t(i,j,k,1,inert(2)) = j
!           t(i,j,k,1,inert(3)) = -1 * GRID(ng) % z_r(i,j,k)
!           t(i,j,k,1,inert(4)) = GRID(ng) % h(i,j)
!           if ( GRID(ng)%h(i,j) .le. 90.0_r8 ) then
!             t(i,j,k,1,inert(5)) = 1.0_r8
!           else
!             t(i,j,k,1,inert(5)) = 0.0_r8
!           endif
!           DO ip=1,NPT
!             itrc=inert(ip)
!             t(i,j,k,2,itrc)=t(i,j,k,1,itrc)
!           END DO
!         END DO
!       END DO
!     END DO
      ip = 1
      DO k=1,N(ng)
        DO j=JstrT,JendT
          DO i=IstrT,IendT
            itrc=inert(ip)
            t(i,j,k,1,itrc) = 0.
	    if (GRID(ng)%z_r(i,j,k) < -20.) then
              t(i,j,k,1,itrc) = 1.
	    endif
            t(i,j,k,2,itrc)=t(i,j,k,1,itrc)
          END DO
        END DO
      END DO
      DO ip=2,NPT
        DO k=1,N(ng)
          DO j=JstrT,JendT
            DO i=IstrT,IendT
              itrc=inert(ip)
              t(i,j,k,1,itrc) = 0.
              t(i,j,k,2,itrc)=t(i,j,k,1,itrc)
            END DO
          END DO
        END DO
      END DO

      RETURN
      END SUBROUTINE ana_passive_tile
