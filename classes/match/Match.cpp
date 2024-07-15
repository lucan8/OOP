#include "Match.h"

void Match :: setPositions(){
    //Size of position buffer for a team
    uint16_t team_pos_size = Constants :: getVal("MATCH_TEAM_SIZE") * Constants :: getVal("TRIANGLE_VERTICES")
                                * Constants :: getVal("NR_COORDS_VERTEX");
    //Allocating the buffer for player positions
    this->player_positions = unique_ptr<float>(new float[team_pos_size * 2]);
    //Getting the positions buffer for the first team                       
    unique_ptr<float> t1_p_pos = this->t1->getTrianglePositions(MatchPlayer :: pitch_half :: first);
    //Copying the first team's positions at the beginning of the buffer
    memcpy(this->player_positions.get(), t1_p_pos.get(), team_pos_size * sizeof(float));
    //Changing player's positions to the other half of the pitch
    t2->changeSide();
    //Getting the positions buffer for the second team
    unique_ptr<float> t2_p_pos = this->t2->getTrianglePositions(MatchPlayer :: pitch_half :: second);
    //Copying the second team's positions at the end of the buffer
    memcpy(this->player_positions.get() + team_pos_size, t2_p_pos.get(), team_pos_size * sizeof(float));
}

void Match :: createField(){
        //Loading the shaders
        const string shader_path = (filesystem::current_path().parent_path() / "resources" / "shaders" / "").string();
        Shader  pitch_shader((shader_path + "vertex_shader.glsl"),
                            (shader_path + "fragment_shader.glsl")),
                middle_line_shader((shader_path + "vertex_shader.glsl"),
                            (shader_path + "fragment_shader2.glsl")),
                player_shader((shader_path + "vertex_shader.glsl"),
                            (shader_path + "fragment_shader1.glsl"));
        
        VAO vao;
        vao.bind();

        uint16_t pitch_vert_size = Constants :: getVal("PITCH_VERTICES") * Constants :: getVal("NR_COORDS_VERTEX");
        uint16_t team_pos_size = Constants :: getVal("MATCH_TEAM_SIZE") * Constants :: getVal("TRIANGLE_VERTICES")
                                 * Constants :: getVal("NR_COORDS_VERTEX");
        
        //Already bound on creation
        VBO static_vbo(Constants :: getVertexPositions("PITCH"), pitch_vert_size * sizeof(float), GL_STATIC_DRAW),
            dynamic_vbo(this->player_positions.get(), team_pos_size * 2 * sizeof(float), GL_DYNAMIC_DRAW);
        
        GLuint indices[6] = {
            //Used for pitch, middle line and players
            0, 1, 2,
            0, 2, 3
        };
        IBO ibo(indices, 6 * sizeof(GLuint));

        //Drawing the pitch
        pitch_shader.use();
        static_vbo.bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
        
        ibo.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //Drawing the players
        player_shader.use();
        dynamic_vbo.bind();
        for (int i = 0; i < Constants :: getVal("MATCH_TEAM_SIZE") * 2; ++i){
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)((i * 6) * sizeof(float)));
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        }
}