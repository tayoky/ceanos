# Contributing to CeanOS

## Rules:

1. **Do NOT Remove Code**: Do not delete any existing code. If you encounter issues, focus on fixing them without removing functionality, as each piece of code likely has a purpose.
  
2. **Maintain Code Style**: Follow the established coding style for consistency. CeanOS has a specific format, so please adhere to spacing, naming conventions, and comments to match the current codebase.

3. **Test Thoroughly**: Before submitting your changes, test them rigorously. Run tests across different setups (if possible) and check for any regressions in core functionalities, especially in hardware interactions or core kernel functions.

4. **Document New Features**: If you add a feature, update or create documentation so others can understand and use it easily. This includes comments within the code, as well as updating relevant documentation files.

5. **Avoid Large Dependencies**: Avoid adding large external libraries or dependencies. If you must add a dependency, ensure it’s justified and discuss it with the maintainers first.

6. **Clearly Describe Commits**: Use clear and detailed commit messages. Describe the purpose of each change briefly but specifically (e.g., “Fixed keyboard driver key mapping” or “Added paging support”).

7. **Discuss Major Changes First**: If you're planning a significant change or addition, create an issue or discussion first to align with the maintainers. This avoids duplicate work or unnecessary code rewrites.

8. **Check Compatibility**: Ensure compatibility with the supported architecture (e.g., x86). If you're adding support for a new architecture, make sure it's isolated and doesn’t interfere with existing implementations.

9. **Performance Matters**: CeanOS is performance-oriented, so prioritize efficiency. Optimize where possible, and avoid bloating the code with unnecessary checks or complex structures.

10. **Ask Questions If Needed**: If something is unclear, don’t hesitate to ask! It’s better to clarify than to introduce errors into the codebase. Reach out via issues or discussions to ensure you’re on the right track.

## Pull Request Process:

1. **Fork the Repository**: Start by forking CeanOS to your GitHub account and clone it locally for your changes.
  
2. **Create a New Branch**: Work in a feature or fix-specific branch (e.g., `fix-keyboard-driver` or `add-paging-support`). Avoid making changes directly in the `main` branch of your fork.

3. **Submit PR with Details**: When your changes are ready, submit a Pull Request with a clear title and description. Mention any specific tests you performed and note any known limitations of your changes.

4. **Wait for Review**: A maintainer will review your PR. Be prepared to make adjustments based on feedback and to explain your reasoning for any complex code sections.

Thank you for contributing to CeanOS!
