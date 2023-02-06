#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(3);
	ofEnableDepthTest();

	this->font.loadFont("fonts/msgothic.ttc", 50, true, true, true);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.7);
	ofRotateX(ofGetFrameNum() * 1.2);

	ofColor color;
	string word = "JunKiyoshi";
	
	for (int i = 0; i < 300; i++) {

		ofRotateZ(ofRandom(360));
		ofRotateY(ofRandom(360));
		ofRotateX(ofRandom(360));

		auto noise_value = ofNoise(ofRandom(1000), ofGetFrameNum() * 0.05);
		auto len = noise_value < 0.5 ? 0 : ofMap(noise_value, 0.5, 1, 0, 150);

		int sample_count = 60;

		auto hue = ofRandom(255);
		ofPath chara_path = this->font.getCharacterAsPoints(word[(int)ofRandom(word.size())], true, false);
		vector<ofPolyline> outline = chara_path.getOutline();

		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(sample_count);
			vector<glm::vec3> vertices = outline[outline_index].getVertices();
			vector<glm::vec3> mesh_vertices;
			vector<glm::vec3> base_location_list;

			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				auto base_location = glm::vec3(0, 0, -150);
				auto location = vertices[vertices_index] - glm::vec3(25, -25, 0);

				mesh_vertices.push_back(location);
				base_location_list.push_back(base_location);
			}

			ofMesh face, line;
			line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
			for (int k = 0; k < mesh_vertices.size(); k++) {

				face.addVertex(base_location_list[k] + mesh_vertices[k] + glm::vec3(0, 0, 0));
				face.addVertex(base_location_list[k] + mesh_vertices[k] + glm::vec3(0, 0, -len));

				line.addVertex(base_location_list[k] + mesh_vertices[k] + glm::vec3(0, 0, 0));
				line.addVertex(base_location_list[k] + mesh_vertices[k] + glm::vec3(0, 0, -len));

				if (k > 0) {

					face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(face.getNumVertices() - 4);
					face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 3); face.addIndex(face.getNumVertices() - 4);

					line.addIndex(line.getNumVertices() - 1); line.addIndex(line.getNumVertices() - 3);
					line.addIndex(line.getNumVertices() - 2); line.addIndex(line.getNumVertices() - 4);
				}
			}

			face.addIndex(face.getNumVertices() - 1); face.addIndex(0); face.addIndex(1);
			face.addIndex(face.getNumVertices() - 1); face.addIndex(face.getNumVertices() - 2); face.addIndex(2);

			line.addIndex(line.getNumVertices() - 1); line.addIndex(1);
			line.addIndex(line.getNumVertices() - 2); line.addIndex(0);

			color.setHsb(hue, 150, 230);

			ofSetColor(color);
			face.draw();

			color.setHsb(hue, 255, 255);

			ofSetColor(ofColor(255));
			line.drawWireframe();
		}
	}

	ofSetColor(0);
	ofDrawSphere(145);

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}