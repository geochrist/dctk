import sys
import os
import numpy as np

def populate_gmat_res( lst_resistors, gmat ):
    (nrows, ncols) = gmat.shape

    # just make sure the matrix is all zeros
    for i in range(nrows):
        for j in range(ncols):
            gmat[i][j] = 0.0

    for each_res in lst_resistors:
        val = each_res[0]
        gval = 1.0/val
        n1 = each_res[1] - 1
        n2 = each_res[2] - 1

        if n1 >= 0:
            gmat[n1][n1] = gmat[n1][n1] + gval
        if n2 >= 0:
            gmat[n2][n2] = gmat[n2][n2] + gval

        if n1 >= 0 and n2 >= 0:
            gmat[n1][n2] = gmat[n1][n2] - gval
            gmat[n2][n1] = gmat[n2][n1] - gval


    return
        

def populate_rhs_mat( lst_isources, imat ):
    (nrows, ncols) = imat.shape

    # ncols shoudl be 1
    for i in range(nrows):
        imat[i] = 0.0

    for each_isrc in lst_isources:
        val = each_isrc[0]
        n1 = each_isrc[1] - 1
        n2 = each_isrc[2] - 1

        if n1 >= 0:
            imat[n1] = imat[n1] + val
        if n2 >= 0:
            imat[n2] = imat[n2] - val

    return


def read_ckt( ckt_file ):
    ifp = open(ckt_file, "r")
    all_lines = ifp.readlines()
    ifp.close()

    cur_src = list()
    all_res = list()
    all_cap = list()
    for each_line in all_lines:
        each_line.rstrip()
        line_pcs = each_line.split()
        if "isrc" in line_pcs[0]:
            in1 = int(line_pcs[1])
            in2 = int(line_pcs[2])
            cur_src.append(in1)
            cur_src.append(in2)
            for j in range(3, len(line_pcs), 2):
                tval = float(line_pcs[j])
                ival = float(line_pcs[j+1])
                cur_src.append((tval, ival))
        elif "res" in line_pcs[0]:
            in1 = int(line_pcs[1])
            in2 = int(line_pcs[2])
            val = float(line_pcs[3])
            all_res.append((val, in1, in2))
        elif "cap" in line_pcs[0]:
            in1 = int(line_pcs[1])
            in2 = int(line_pcs[2])
            val = float(line_pcs[3])
            all_cap.append((val, in1, in2))
        else:
            print("Error: unknown circuit device on line %s" %(each_line))

    return cur_src, all_res, all_cap


def create_system( all_res ):
    # identify the highest node number

    max_node = 0
    for each_res in all_res:
        n1 = each_res[1]
        n2 = each_res[2]
        max_node = max([max_node, n1, n2])

    nrows = max_node
    ncols = max_node
    gmatrix = np.zeros((nrows, ncols))
    imatrix = np.zeros((nrows,1))
    vmatrix = np.zeros((nrows,1))
    v0matrix = np.zeros((nrows,1))

    return nrows, ncols, gmatrix, imatrix, vmatrix, v0matrix


def solve_one_step( gmatrix, imatrix, all_res, all_cap, v0matrix, isrc_val, tstep, cap_currents ):
    # first create the list of all "conductances"
    all_devs = list()
    for each_res in all_res:
        all_devs.append((each_res[0], each_res[1], each_res[2]))

    all_srcs = list()
    # insert the active source at next time value
    all_srcs.append( (isrc_val, 1, 0) ) 
    # insert all cap device parallel current sources
    new_cap_currents = list()
    num_caps = len(all_cap)
    for i in range(num_caps):
        each_cap = all_cap[i]

        rval = tstep / each_cap[0] / 2.0
        n1 = each_cap[1] - 1
        n2 = each_cap[2] - 1
        all_devs.append((rval, each_cap[1], each_cap[2]))
        ival = float(v0matrix[n1]) / rval
        if n2>=0:
            ival = ival - float(v0matrix[n2]) / rval
        ival = 2.0*ival - cap_currents[i]
        new_cap_currents.append(ival)
        all_srcs.append((ival, each_cap[1], each_cap[2]))

    # print("AllDevs: %s" %(str(all_devs)))
    # print("AllIsrc: %s" %(str(all_srcs)))

    # populate the system of equations
    populate_gmat_res( all_devs, gmatrix )
    populate_rhs_mat( all_srcs, imatrix )
    nrows, ncols = gmatrix.shape

    # print("System:")
    # for i in range(nrows):
    #     print("%d %s | %s" %(i, str(gmatrix[i]), str(imatrix[i])))

    # solve the system
    vmatrix = np.linalg.solve(gmatrix, imatrix)

    # print("Sol: %s" %(str(vmatrix)))
    return vmatrix, new_cap_currents

def get_detailed_active_cur_vals( cur_lst, tstep ):
    # interpolate the values from the current source pwl definition with the given time step
    # return the interpolated current values at every time step
    x_t = list()
    y_i = list()
    npairs = len(cur_lst)
    for i in range(2, npairs):
        ith_pair = cur_lst[i]
        x_t.append(ith_pair[0])
        y_i.append(ith_pair[1])

    max_t = x_t[-1]
    min_t = x_t[0]
    cur_t = min_t
    max_t = max_t + 2.0 * (max_t - min_t)


    detail_tvals = np.arange(min_t, max_t, tstep)
    detail_ivals = list()

    num_pts = int( (max_t - min_t) / tstep )
    num_brk_pts = len(x_t)
    for i  in range(num_pts):
        tval = detail_tvals[i]
        inside_section = False
        for j in range( num_brk_pts-1 ):
            if tval >= x_t[j] and tval < x_t[j+1]:
                ival = y_i[j] + (tval - x_t[j]) / (x_t[j+1] - x_t[j]) * (y_i[j+1] - y_i[j])
                detail_ivals.append(ival)
                inside_section = True
        if inside_section == False:
            detail_ivals.append( y_i[-1] )

    # print("Detailed source:")
    # print("T: %s" %(str(detail_tvals)))
    # print("I: %s" %(str(detail_ivals)))
    
    return min_t, max_t, detail_ivals



def get_output_signal( cur_src, all_res, all_cap, tstep ):
    
    start_time, stop_time, detailed_ivals = get_detailed_active_cur_vals( cur_src, tstep )

    # create the empty system of equations
    nrows, ncols, gmatrix, imatrix, vmatrix, v0matrix = create_system( all_res )

    # integrate, step by step
    sol_node_v = list()
    sol_node_t = list()
    cur_time = start_time
    num_tsteps = int((stop_time - start_time)/tstep)

    # we assume the src is connected to node 1 and 0
    sol_node_v.append( vmatrix[0] )
    sol_node_t.append( cur_time )
    prev_cap_currents = list()
    for each_cap in all_cap:
        prev_cap_currents.append(0.0)

    # at the start, all nodes are at 0 voltage so v0matrix is the correct initial point
    for tindx in range(1,num_tsteps):
        src_ival = detailed_ivals[tindx]
        vmatrix, new_cap_currents = solve_one_step( gmatrix, imatrix, all_res, all_cap, v0matrix, src_ival, tstep, prev_cap_currents )

        sol_node_v.append( float(vmatrix[0]) )

        v0matrix = vmatrix
        prev_cap_currents = new_cap_currents

        cur_time = cur_time + tstep
        sol_node_t.append( cur_time )

    output_waveform = list()
    for i in range(num_tsteps):
        output_waveform.append((sol_node_t[i], sol_node_v[i]))

    return output_waveform

def main():

    # read the input file 
    ckt_file = sys.argv[1]
    cur_src, all_res, all_cap = read_ckt( ckt_file )    

    # create detailed current source
    tstep = float( sys.argv[2] )

    # simulate the circuit
    output_waveform = get_output_signal( cur_src, all_res, all_cap, tstep )

    # print the waveform
    print("Final output voltage waveform:")
    for each_point in output_waveform:
        print("%g %g" %( each_point[0], each_point[1] ))

    return

if __name__ == '__main__':
    main()

    quit()
    

