#include "objparser.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

using namespace std;

namespace {
    vector<string> split_by(const string &line, const char c) {
        vector<string> out;
        stringstream ss(line);
        string segment;
        while (getline(ss, segment, c)) {
            out.push_back(segment);
        }
        return out;
    }

    vector<string> split_by_whitespace(const string &line) {
        return split_by(line, ' ');
    }

    vector<GLfloat> line_to_vertices(const string &line) {
        vector<GLfloat> vertices(3);
        vector<string> parts = split_by_whitespace(line);
        assert(parts.size() == 4);
        int counter = 0;
        for (int i=1; i<4; i++) {
            vertices[counter] = atof(parts[i].c_str());
            counter++;
        }

        return vertices;
    }

    void parse_vertex(const string &line, ObjModel &model) {
        vector<GLfloat> vertices = line_to_vertices(line);
        for (auto value: vertices) model.vertices.push_back(value);
    }

    void parse_normal(const string &line, ObjModel &model) {
        vector<GLfloat> vertices = line_to_vertices(line);
        for (auto value: vertices) model.normals.push_back(value);
    }

    void parse_face(const string &line, ObjModel &model) {
        vector<string> parts = split_by_whitespace(line);
        assert(parts.size() == 4);
        for (int i=1; i<4; i++) {
            vector<string> part = split_by(parts[i], '/');
            model.indexes.push_back(atoi(part[0].c_str()));
        }
    }

    void parse_line(const string &line, ObjModel &model, int pass) {
        switch (line[0]) {
            case 'v':
                if (pass == 1) {
                    return;
                }
                if (line[1] == 'n') {
                    parse_normal(line, model);
                } else {
                    parse_vertex(line, model);
                }
                break;
            case 'f':
                if (pass == 0) {
                    return;
                }
                parse_face(line, model);
                break;
            default:
                return;
        }
    }
}

ObjModel ObjParser::model(const string &filename) {
    ObjModel model;
    for (int pass=0; pass<2; pass++) {
        ifstream infile(filename.c_str());
        if (!infile.is_open()) {
            throw runtime_error("Cannot read object file");
        }
        string line;
        while (getline(infile, line)) {
            parse_line(line, model, pass);
        }
    }
    return model;
}
