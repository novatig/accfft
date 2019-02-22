/**
 * @file
 * Single Precision CPU functions of AccFFT operators
 */
/*
 *  Copyright (c) 2014-2015, Amir Gholami, George Biros
 *  All rights reserved.
 *  This file is part of AccFFT library.
 *
 *  AccFFT is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  AccFFT is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AccFFT.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <cmath>
#include <math.h>
#include <string.h>
#include <accfftf.h>
#include <accfft_operators.h>

#include "operators.txx"

template void grad_mult_wave_numberx<Complexf>(Complexf* wA, Complexf* A,
    int* N, MPI_Comm c_comm, int* size, int* start, std::bitset<3> xyz);
template void grad_mult_wave_numbery<Complexf>(Complexf* wA, Complexf* A,
    int* N, MPI_Comm c_comm, int* size, int* start, std::bitset<3> xyz);
template void grad_mult_wave_numberz<Complexf>(Complexf* wA, Complexf* A,
    int* N, MPI_Comm c_comm, int* start, int* size, std::bitset<3> xyz);
template void grad_mult_wave_number_laplace<Complexf>(Complexf* wA, Complexf* A,
    int* N, MPI_Comm c_comm);
template void biharmonic_mult_wave_number<Complexf>(Complexf* wA, Complexf* A,
    int* N, MPI_Comm c_comm);

template void grad_mult_wave_numberx_inplace<Complexf>(Complexf* A,
    int* N, MPI_Comm c_comm, int* size, int* start, std::bitset<3> xyz);
template void grad_mult_wave_numbery_inplace<Complexf>(Complexf* A,
    int* N, MPI_Comm c_comm, int* size, int* start, std::bitset<3> xyz);
template void grad_mult_wave_numberz_inplace<Complexf>(Complexf* A,
    int* N, MPI_Comm c_comm, int* start, int* size, std::bitset<3> xyz);
template void grad_mult_wave_number_laplace_inplace<Complexf>(Complexf* A,
    int* N, MPI_Comm c_comm);
template void biharmonic_mult_wave_number_inplace<Complexf>(Complexf* A,
    int* N, MPI_Comm c_comm);



template void accfft_grad_slow_t<float, accfft_planf>(float * A_x, float *A_y,
    float* A_z, float* A, accfft_planf *plan, std::bitset<3>* pXYZ,
    double* timer);
template void accfft_grad_t<float, accfft_planf>(float * A_x, float *A_y,
    float* A_z, float* A, accfft_planf *plan, std::bitset<3>* pXYZ,
    double* timer);
template void accfft_laplace_t<float, accfft_planf>(float * LA, float *A,
    accfft_planf *plan, double* timer);
template void accfft_divergence_slow_t<float, accfft_planf>(float* divA, float* A_x,
    float* A_y, float*A_z, accfft_planf *plan, double* timer);
template void accfft_divergence_t<float, accfft_planf>(float* divA, float* A_x,
    float* A_y, float*A_z, accfft_planf *plan, double* timer);
template void accfft_biharmonic_t<float, accfft_planf>(float * BA, float *A,
    accfft_planf *plan, double* timer);

template void accfft_grad_slow_t<float, accfft_plantf>(float * A_x, float *A_y,
    float* A_z, float* A, accfft_plantf *plan, std::bitset<3>* pXYZ,
    double* timer);
template void accfft_grad_t<float, accfft_plantf>(float * A_x, float *A_y,
    float* A_z, float* A, accfft_plantf *plan, std::bitset<3>* pXYZ,
    double* timer);
template void accfft_divergence_t<float, accfft_plantf>(float* divA, float* A_x,
    float* A_y, float*A_z, accfft_plantf *plan, double* timer);
template void accfft_laplace_t<float, accfft_plantf>(float * LA, float *A,
    accfft_plantf *plan, double* timer);
template void accfft_divergence_slow_t<float, accfft_plantf>(float* divA, float* A_x,
    float* A_y, float*A_z, accfft_plantf *plan, double* timer);
template void accfft_biharmonic_t<float, accfft_plantf>(float * BA, float *A,
    accfft_plantf *plan, double* timer);

/**
 * Computes single precision gradient of its input real data A, and returns the x, y, and z components
 * and writes the output into A_x, A_y, and A_z respectively.
 * @param A_x The x component of \f$\nabla A\f$
 * @param A_y The y component of \f$\nabla A\f$
 * @param A_z The z component of \f$\nabla A\f$
 * @param plan FFT plan created by \ref accfft_plan_dft_3d_r2cf. Must be an outplace plan, otherwise the function will return
 * without computing the gradient.
 * @param pXYZ a bit set pointer field of size 3 that determines which gradient components are needed. If XYZ={111} then
 * all the components are computed and if XYZ={100}, then only the x component is computed. This can save the user
 * some time, when just one or two of the gradient components are needed.
 * @param timer See \ref timer for more details.
 */
void accfft_gradf(float * A_x, float *A_y, float* A_z, float* A,
    accfft_planf *plan, std::bitset<3> *pXYZ, double* timer) {
  accfft_grad_t<float, accfft_planf>(A_x, A_y, A_z, A, plan, pXYZ, timer);

}

/**
 * Computes single precision Laplacian of its input real data A,
 * and writes the output into LA.
 * @param LA  \f$\Delta A\f$
 * @param plan FFT plan created by \ref accfft_plan_dft_3d_r2cf. Must be an outplace plan, otherwise the function will return
 * without computing the gradient.
 * @param timer See \ref timer for more details.
 */
void accfft_laplacef(float * LA, float *A, accfft_planf *plan, double* timer) {
  accfft_laplace_t<float, accfft_planf>(LA, A, plan, timer);
}

/**
 * Computes single precision divergence of its input vector data A_x, A_y, and A_x.
 * The output data is written to divA.
 * @param divA  \f$\nabla\cdot(A_x i + A_y j+ A_z k)\f$
 * @param A_x The x component of \f$\nabla A\f$
 * @param A_y The y component of \f$\nabla A\f$
 * @param A_z The z component of \f$\nabla A\f$
 * @param plan FFT plan created by \ref accfft_plan_dft_3d_r2cf. Must be an outplace plan, otherwise the function will return
 * without computing the gradient.
 * @param timer See \ref timer for more details.
 */
void accfft_divergencef(float* divA, float* A_x, float* A_y, float*A_z,
    accfft_planf *plan, double* timer) {
  accfft_divergence_t<float, accfft_planf>(divA, A_x, A_y, A_z, plan, timer);
}

/**
 * Computes single precision Biharmonic of its input real data A,
 * and writes the output into LA.
 * @param BA  \f$\Delta^2 A\f$
 * @param plan FFT plan created by \ref accfft_plan_dft_3d_r2cf. Must be an outplace plan, otherwise the function will return
 * without computing the gradient.
 * @param timer See \ref timer for more details.
 */
void accfft_biharmonicf(float * BA, float *A, accfft_planf *plan,
    double* timer) {
  accfft_biharmonic_t<float, accfft_planf>(BA, A, plan, timer);
}

#ifdef USE_PNETCDF
#include <pnetcdf.h>
#include <cstdlib>
#include <string>
#include <iostream>

#define PNETCDF_HANDLE_ERROR {              \
    if (err != NC_NOERR)              \
      printf("PNetCDF Error at line %d (%s)\n", __LINE__,     \
       ncmpi_strerror(err));            \
}

enum ComponentIndex3D {
  IX = 0,
  IY = 1,
  IZ = 2
};

/**
 * Read a single-precision parallel-nedcdf file.
 *
 * We assume here that localData is a scalar.
 *
 * Pnetcdf uses row-major format (same as FFTW).
 *
 * \param[in]  filename  : PnetCDF filename
 * \param[in]  starts    : offset to where to start reading data
 * \param[in]  counts    : number of elements read (3D sub-domain inside global)
 * \param[in]  gsizes    : global sizes
 * \param[out] localData : actual data buffer (size : nx*ny*nz*sizeof(float))
 *
 * localData must have been allocated prior to calling this routine.
 */
void read_pnetcdf(const std::string &filename,
    MPI_Offset starts[3],
    MPI_Offset counts[3],
    int gsizes[3],
    float *localData)
{

  int myRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  // netcdf file id
  int ncFileId;
  int err;

  // file opening mode
  int ncOpenMode = NC_NOWRITE;

  int nbVar=1;
  int varIds[nbVar];
  MPI_Info mpi_info_used;

  /*
   * Open NetCDF file
   */
  err = ncmpi_open(MPI_COMM_WORLD, filename.c_str(),
      ncOpenMode,
      MPI_INFO_NULL, &ncFileId);
  if (err != NC_NOERR) {
    printf("Error: ncmpi_open() file %s (%s)\n",filename.c_str(),ncmpi_strerror(err));
    MPI_Abort(MPI_COMM_WORLD, -1);
    exit(1);
  }

  /*
   * Query NetCDF mode
   */
  int NC_mode;
  err = ncmpi_inq_version(ncFileId, &NC_mode);
  if (myRank==0) {
    if (NC_mode == NC_64BIT_DATA)
    std::cout << "Pnetcdf Input mode : NC_64BIT_DATA (CDF-5)\n";
    else if (NC_mode == NC_64BIT_OFFSET)
    std::cout << "Pnetcdf Input mode : NC_64BIT_OFFSET (CDF-2)\n";
    else
    std::cout << "Pnetcdf Input mode : unknown\n";
  }

  /*
   * Query information about variable named "data"
   */
  {
    int ndims, nvars, ngatts, unlimited;
    err = ncmpi_inq(ncFileId, &ndims, &nvars, &ngatts, &unlimited);
    PNETCDF_HANDLE_ERROR;

    err = ncmpi_inq_varid(ncFileId, "data", &varIds[0]);
    PNETCDF_HANDLE_ERROR;
  }

  /*
   * Define expected data types (no conversion done here)
   */
  MPI_Datatype mpiDataType = MPI_FLOAT;

  /*
   * Get all the MPI_IO hints used (just in case, we want to print it after
   * reading data...
   */
  err = ncmpi_get_file_info(ncFileId, &mpi_info_used);
  PNETCDF_HANDLE_ERROR;

  /*
   * Read heavy data (take care of row-major / column major format !)
   */
  int nItems = counts[IX]*counts[IY]*counts[IZ];
  {

    err = ncmpi_get_vara_all(ncFileId,
        varIds[0],
        starts,
        counts,
        localData,
        nItems,
        mpiDataType);
    PNETCDF_HANDLE_ERROR;
  } // end reading heavy data

  /*
   * close the file
   */
  err = ncmpi_close(ncFileId);
  PNETCDF_HANDLE_ERROR;

} // read_pnetcdf

/**
 * Write a parallel-nedcdf file.
 *
 * We assume here that localData is a scalar.
 *
 * Pnetcdf uses row-major format (same as FFTW).
 *
 * \param[in]  filename  : PnetCDF filename
 * \param[in]  starts    : offset to where to start reading data
 * \param[in]  counts    : number of elements read (3D sub-domain inside global)
 * \param[in]  gsizes    : global sizes
 * \param[in]  localData : actual data buffer (size : nx*ny*nz*sizeof(float))
 *
 */
void write_pnetcdf(const std::string &filename,
    MPI_Offset starts[3],
    MPI_Offset counts[3],
    int gsizes[3],
    float *localData)
{
  int myRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  // netcdf file id
  int ncFileId;
  int err;

  // file creation mode
  int ncCreationMode = NC_CLOBBER;

  // CDF-5 is almost mandatory for very large files (>= 2x10^9 cells)
  // not useful here
  bool useCDF5 = false;
  if (useCDF5)
  ncCreationMode = NC_CLOBBER|NC_64BIT_DATA;
  else// use CDF-2 file format
  ncCreationMode = NC_CLOBBER|NC_64BIT_OFFSET;

  // verbose log ?
  //bool pnetcdf_verbose = false;

  int nbVar=1;
  int dimIds[3], varIds[nbVar];
  //MPI_Offset write_size, sum_write_size;
  MPI_Info mpi_info_used;
  //char str[512];

  // time measurement variables
  //float write_timing, max_write_timing, write_bw;

  /*
   * Create NetCDF file
   */
  err = ncmpi_create(MPI_COMM_WORLD, filename.c_str(),
      ncCreationMode,
      MPI_INFO_NULL, &ncFileId);
  if (err != NC_NOERR) {
    printf("Error: ncmpi_create() file %s (%s)\n",filename.c_str(),ncmpi_strerror(err));
    MPI_Abort(MPI_COMM_WORLD, -1);
    exit(1);
  }

  /*
   * Define global dimensions
   */
  err = ncmpi_def_dim(ncFileId, "x", gsizes[0], &dimIds[0]);
  PNETCDF_HANDLE_ERROR;

  err = ncmpi_def_dim(ncFileId, "y", gsizes[1], &dimIds[1]);
  PNETCDF_HANDLE_ERROR;

  err = ncmpi_def_dim(ncFileId, "z", gsizes[2], &dimIds[2]);
  PNETCDF_HANDLE_ERROR;

  /*
   * Define variables to write (give a name)
   */
  nc_type ncDataType = NC_FLOAT;
  MPI_Datatype mpiDataType = MPI_FLOAT;

  err = ncmpi_def_var(ncFileId, "data", ncDataType, 3, dimIds, &varIds[0]);
  PNETCDF_HANDLE_ERROR;

  /*
   * global attributes
   */
  // did we use CDF-2 or CDF-5
  {
    int useCDF5_int = useCDF5 ? 1 : 0;
    err = ncmpi_put_att_int(ncFileId, NC_GLOBAL, "CDF-5 mode", NC_INT, 1, &useCDF5_int);
    PNETCDF_HANDLE_ERROR;
  }

  /*
   * exit the define mode
   */
  err = ncmpi_enddef(ncFileId);
  PNETCDF_HANDLE_ERROR;

  /*
   * Get all the MPI_IO hints used
   */
  err = ncmpi_get_file_info(ncFileId, &mpi_info_used);
  PNETCDF_HANDLE_ERROR;

  // copy data to write in intermediate buffer
  int nItems = counts[IX]*counts[IY]*counts[IZ];

  {

    // debug
    // printf("Pnetcdf [rank=%d] starts=%lld %lld %lld, counts =%lld %lld %lld, gsizes=%d %d %d\n",
    //     myRank,
    //     starts[0],starts[1],starts[2],
    //     counts[0],counts[1],counts[2],
    //     gsizes[0],gsizes[1],gsizes[2]);

    /*
     * make sure PNetCDF doesn't complain when starts is outside of global domain
     * bound. When nItems is null, off course we don't write anything, but starts
     * offset have to be inside global domain.
     * So there is no harm, setting starts to origin.
     */
    if (nItems == 0) {
      starts[0]=0;
      starts[1]=0;
      starts[2]=0;
    }

    err = ncmpi_put_vara_all(ncFileId,
        varIds[0],
        starts,
        counts,
        localData,
        nItems,
        mpiDataType);
    PNETCDF_HANDLE_ERROR;
  }

  /*
   * close the file
   */
  err = ncmpi_close(ncFileId);
  PNETCDF_HANDLE_ERROR;

} // write_pnetcdf
#endif
