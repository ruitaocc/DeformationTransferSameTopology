#ifndef __OBJMESH_H__
#define __OBJMESH_H__

#include "Renderable.h"
#include <vector>
#include "bmesh.h"
class ObjMesh : public Renderable
{
public:
	const static int MAX_VERT_COUNT = 4;
	const static int MATERIAL_NAME_SIZE = 255;
	const static int OBJ_FILENAME_LENGTH = 500;
	const static int OBJ_LINE_SIZE = 500;
	typedef struct obj_face
	{
		int vertex_index[MAX_VERT_COUNT];
		int normal_index[MAX_VERT_COUNT];
		int texture_index[MAX_VERT_COUNT];
		int vertex_count;
		int material_index;
	};

	typedef struct obj_material
	{
		char name[MATERIAL_NAME_SIZE];
		char texture_filename[OBJ_FILENAME_LENGTH];
		unsigned int texture_id;
		std::vector<unsigned char> image;
		int image_width, image_height, image_channel;
		ldp::Float3 amb;
		ldp::Float3 diff;
		ldp::Float3 spec;
		float  reflect;
		float  refract;
		float  trans;
		float  shiny;
		float  glossy;
		float  refract_index;
		obj_material()
		{
			texture_id = 0;
			image_width = 0;
			image_height = 0;
			image_channel = 0;
			amb[0] = 0.0f;
			amb[1] = 0.0f;
			amb[2] = 0.0f;
			diff[0] = 0.8f;
			diff[1] = 0.8f;
			diff[2] = 0.8f;
			spec[0] = 0.0f;
			spec[1] = 0.0f;
			spec[2] = 0.0f;
			reflect = 0.0f;
			refract = 0.0f;
			trans = 1;
			glossy = 98;
			shiny = 0;
			refract_index = 1;
			texture_filename[0] = '\0';
		}
		void drawMat(int isTextureEnabled)const;
		void generateTextures();
	};
public:
	ObjMesh();
	ObjMesh(const ObjMesh& rhs);
	~ObjMesh();
	virtual void clear();
	virtual void render(int showType, int frameIndex=0);
	virtual void renderConstColor(ldp::Float3 color)const;
	virtual int loadObj(const char* path, bool isNormalGen, bool isNormalize);
	virtual void saveObj(const char* path)const;
	int loadOff(const char* filename, bool isNormalize);
	void updateNormals();
	void flipNormals();
	void updateBoundingBox();
	void normalizeModel();
	void requireRenderUpdate();

	enum VertexSelectOP
	{
		Select_OnlyGiven,
		Select_Union,
		Select_Remove,
	};
	void setSelection(const std::vector<int>& selectedIds);
	void getSelection(std::vector<int>& selectedIds)const;
	bool isVertexSelected(int i);
	void selectSingleVertex(int vert_id, VertexSelectOP op);
	void selectLinkedVertices(int vert_id, VertexSelectOP op);
	void selectAll();
	void selectNone();
	void selectShortestPath(int vert_id_1, int vert_id_2, bool disablePathIntersect = true);
	void selectInnerRegion(int vert_id);
	bool hasSelectedVert()const;

	virtual void getSubMesh(const std::vector<int>& validVertexIdx, 
		ObjMesh* subMesh, std::vector<int>* faceIdToValidFaceId=0)const;

	bool subdiv_loop_to(ObjMesh& result);

	ObjMesh& operator=(const ObjMesh& rhs);

	virtual void cloneFrom(const ObjMesh* rhs);

	void translate(ldp::Float3 t);
	void scaleByCenter(ldp::Float3 s);
	void scaleBy(ldp::Float3 s, ldp::Float3 c);
	void rotateByCenter(ldp::Mat3f r);
	void rotateBy(ldp::Mat3f r, ldp::Float3 c);
	void transform(ldp::Mat4f T);

	virtual ldp::Float3 getCenter()const { return (boundingBox[0] + boundingBox[1])*0.5f; }
	virtual ldp::Float3 getBoundingBox(int i)const { return boundingBox[i]; }

	// bmesh structure, which is covinient for per-element oparation
	ldp::BMesh* get_bmesh(bool triangulate);
	ldp::BMVert* get_bmesh_vert(int i){ return m_bmeshVerts[i]; }
protected:
	int obj_parse_vertex_index(int *vertex_index, int *texture_index, int *normal_index)const;
	int obj_parse_mtl_file(const char *filename);
	void drawMaterial(int idx)const;
	void renderFaces(int showType)const;
	void generate_fast_view_tri_face_by_group(int showType)const;
	void update_selection_via_bmesh();
public:
	ldp::Float3 boundingBox[2];
	char scene_filename[OBJ_FILENAME_LENGTH];
	char material_filename[OBJ_FILENAME_LENGTH];
	static obj_material default_material;
	std::vector<ldp::Float3> vertex_list;
	std::vector<ldp::Float3> vertex_normal_list;
	std::vector<ldp::Float3> vertex_color_list;
	std::vector<ldp::Float3> face_normal_list;
	std::vector<ldp::Float2> vertex_texture_list;
	std::vector<obj_face> face_list;
	std::vector<obj_material> material_list;
protected:
	mutable std::vector<std::vector<ldp::Float3>> _fast_view_verts;
	mutable std::vector<std::vector<ldp::Float3>> _fast_view_normals;
	mutable std::vector<std::vector<ldp::Float2>> _fast_view_texcoords;
	mutable std::vector<std::vector<ldp::Float3>> _fast_view_colors;
	mutable bool _fast_view_should_update;
	mutable int _fast_view_last_showType;
private:
	ldp::BMesh* m_bmesh;
	bool m_bmesh_triagulate;
	std::vector<ldp::BMVert*> m_bmeshVerts;
	std::vector<int> vertex_is_selected;
};//class ObjMesh


#endif//__OBJMESH_H__