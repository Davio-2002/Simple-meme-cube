#pragma once

#include <third-party/glm/glm.hpp>
#include <third-party/glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::mat4 GetModelMatrix() const
    {
        glm::mat4 model{ 1.0f };

        // translate
        model = glm::translate(model, position);

        // rotate (Euler XYZ, in degrees)
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // scale
        model = glm::scale(model, scale);

        return model;
    }

    void Move(const glm::vec3& delta) { position += delta; }
    void Rotate(const glm::vec3& deltaDegrees) { rotation += deltaDegrees; }
    void Scale(const glm::vec3& factor) { scale *= factor; }

    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };  // degrees: pitch (x), yaw (y), roll (z)
    glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
};
