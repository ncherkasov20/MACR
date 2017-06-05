#include "stdafx.h"

#include "Vertex.h"

int & Vertex::Edge::getEdgeDir() { return m_edgeDir; }

int & Vertex::Edge::getEdgeId() { return m_edgeId; }

int & Vertex::Edge::getEdgeType() { return m_edgeType; }

Vertex * Vertex::Edge::getEdgeVer() { return m_edgeVer; }