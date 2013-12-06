/*	shadow volume mesh
 *
 *		written by Alexander Zaprjagaev
 *		frustum@public.tsu.ru
 */

#ifndef __SV_MESH_H__
#define __SV_MESH_H__

typedef struct {
	float xyz[3];				/* ���������� */
	float normal[3];			/* ������� */
	float st[2];				/* ���������� ���������� */
} vertex_t;

typedef struct {
	float xyz[3];				/* ���������� */
} sv_vertex_t;

typedef struct {
	sv_vertex_t *v0,*v1;		/* ������� */
	int rv;						/* ��� ����������� ����������� ������ */
	int id;						/* ������������� */
} sv_edge_t;

typedef struct {
	sv_vertex_t *v0,*v1,*v2;	/* ������� */
	sv_edge_t *e0,*e1,*e2;		/* ����� */
	int r0,r1,r2;				/* ������ */
	float plane[4];				/* ��������� */
} sv_face_t;

typedef struct {
	int num_vertex;				/* ����� ������ */
	sv_vertex_t *vertex;		/* ������� */
	int num_edge;				/* ����� ����� */
	sv_edge_t *edge;			/* ����� */
	int num_face;				/* ����� ������ */
	sv_face_t *face;			/* ����� */
} sv_t;

typedef struct {
	int num_vertex;				/* ����� ������ */
	vertex_t *vertex;			/* ������� */
	float matrix[16];			/* ������� �������������� */
	sv_t *sv;					/* ������� ����� */
} sv_mesh_t;

sv_mesh_t *sv_mesh_create(float *vertex,int num_vertex);
sv_mesh_t *sv_mesh_load_3ds(char *name);
void sv_mesh_set_matrix(sv_mesh_t *mesh,float *matrix);
void sv_mesh_render(sv_mesh_t *mesh);
void sv_mesh_render_to_stencil(sv_mesh_t *mesh,float *light);
void sv_mesh_render_shadow(float *color);

#endif /* __SV_MESH_H__ */
