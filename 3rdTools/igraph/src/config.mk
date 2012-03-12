product:=igraph
$(product).type:=lib
$(product).product.c.includes:=3rdTools/igraph/include

$(product).c.sources:= $(addprefix $(product-base)/,\
    basic_query.c games.c cocitation.c \
	iterators.c structural_properties.c components.c layout.c \
	structure_generators.c conversion.c measure_dynamics.c \
	type_indexededgelist.c error.c interrupt.c other.c foreign.c \
	random.c attributes.c memory.h random.h \
	dqueue.c heap.c \
	igraph_heap.c igraph_stack.c igraph_strvector.c igraph_trie.c \
	matrix.c vector.c vector_ptr.c memory.c adjlist.c visitors.c \
	igraph_grid.c atlas.c topology.c motifs.c progress.c \
	atlas-edges.h operators.c igraph_psumtree.c array.c \
	igraph_hashtable.c foreign-graphml.c flow.c igraph_buckets.c \
	NetDataTypes.cpp NetRoutines.cpp clustertool.cpp \
	pottsmodel_2.cpp spectral_properties.c cores.c igraph_set.c \
	cliques.c revolver_cit.c revolver_grow.c walktrap.cpp \
	walktrap_heap.cpp walktrap_graph.cpp walktrap_communities.cpp \
	spmatrix.c community.c fast_community.c evolver_cit.c \
	gml_tree.c bliss_bignum.hh bliss_kstack.hh bliss_defs.hh \
	bliss_orbit.cc bliss_eqrefhash.cc bliss_orbit.hh \
	bliss_eqrefhash.hh bliss_partition.cc bliss_graph.cc \
	bliss_partition.hh bliss_graph.hh bliss_timer.cc bliss_heap.cc \
	bliss_timer.hh bliss_heap.hh bliss_utils.cc bliss_kqueue.hh \
	bliss_utils.hh bliss.cc cattributes.c revolver_ml_cit.c \
	zeroin.c bfgs.c math.c forestfire.c arpack/dgetv0.c \
	arpack/dmout.c arpack/dsaitr.c arpack/dsapps.c arpack/dsaup2.c \
	arpack/dsaupd.c arpack/dsconv.c arpack/dseigt.c \
	arpack/dsesrt.c arpack/dseupd.c arpack/dsgets.c \
	arpack/dsortr.c arpack/dstats.c arpack/dstqrb.c arpack/dvout.c \
	arpack/ivout.c arpack/dnaitr.c arpack/dnapps.c arpack/dnaup2.c \
	arpack/dnaupd.c arpack/dnconv.c arpack/dsortc.c \
	arpack/dneigh.c arpack/dngets.c arpack/dstatn.c \
	arpack/dlaqrb.c arpack/dneupd.c arpack/second.c blas/daxpy.c \
	blas/ddot.c blas/dger.c blas/dscal.c blas/lsame.c blas/dcopy.c \
	blas/dgemv.c blas/dnrm2.c blas/dswap.c blas/drot.c \
	blas/idamax.c blas/dtrmm.c blas/dasum.c lapack/xerbla.c \
	lapack/dgeqr2.c lapack/dlarnv.c lapack/dlassq.c \
	lapack/dlacpy.c lapack/dlartg.c lapack/dorm2r.c lapack/dlae2.c \
	lapack/dlaruv.c lapack/dsteqr.c lapack/dlaev2.c \
	lapack/dlascl.c lapack/dlanst.c lapack/dlaset.c lapack/dlarf.c \
	lapack/dlasr.c lapack/dlapy2.c lapack/dlarfg.c lapack/dlasrt.c \
	lapack/dlabad.c lapack/dlanhs.c lapack/dtrevc.c \
	lapack/dlanv2.c lapack/dlaln2.c lapack/dladiv.c \
	lapack/dtrsen.c lapack/dlahqr.c lapack/dlacon.c \
	lapack/dtrsyl.c lapack/dtrexc.c lapack/dlange.c \
	lapack/dlaexc.c lapack/dlasy2.c lapack/dlarfx.c f2c/d_sign.c \
	f2c/etime_.c f2c/pow_dd.c f2c/pow_di.c f2c/s_cmp.c \
	f2c/s_copy.c f2c/d_lg10.c f2c/i_dnnt.c arpack.c centrality.c \
	drl_layout.cpp drl_parse.cpp drl_graph.cpp DensityGrid.cpp \
	gengraph_box_list.cpp gengraph_degree_sequence.cpp \
	gengraph_graph_molloy_hash.cpp \
	gengraph_graph_molloy_optimized.cpp gengraph_mr-connected.cpp \
	gengraph_powerlaw.cpp gengraph_random.cpp bipartite.c \
	drl_layout_3d.cpp drl_graph_3d.cpp DensityGrid_3d.cpp \
    foreign-gml-lexer.c foreign-gml-parser.c \
    foreign-lgl-lexer.c foreign-lgl-parser.c \
    foreign-ncol-lexer.c foreign-ncol-parser.c \
    foreign-pajek-lexer.c foreign-pajek-parser.c \
    )

$(product).c.includes:=3rdTools/igraph/include/igraph \
                       3rdTools/igraph/src

$(product).c.env-includes:=3rdTools/igraph/src

$(product).depends:=xml2
$(product).c.flags.cpp:=-DHAVE_CONFIG_H -DINTERNAL_LAPACK
$(product).c.flags.c:= -O2
$(product).c.flags.ld:=

$(eval $(call product-def,$(product)))
